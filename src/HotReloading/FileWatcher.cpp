#include <SDL3/SDL.h>

#include "FileWatcher.h"
#include "CustomEvents.h"
#include "InotifyWrapper.h"

void watch_directory(const std::string &directory, int build_event_fd,
                     int exit_event_fd) {
  try {
    InotifyWrapper inotify(directory);
    constexpr size_t event_buffer_size = 1024 * (sizeof(inotify_event) + 16);
    char buffer[event_buffer_size];

    // Set up poll on both inotify fd and the exit eventfd.
    struct pollfd fds[2];
    fds[0].fd = inotify.fd();
    fds[0].events = POLLIN;
    fds[1].fd = exit_event_fd;
    fds[1].events = POLLIN;

    while (true) {
      int poll_ret = poll(fds, 2, -1);  // blocking poll
      if (poll_ret < 0) {
        if (errno == EINTR) continue;
        std::cerr << "[ERROR] poll() in watch_directory: "
                  << std::strerror(errno) << "\n";
        break;
      }

      // Check if the exit event was signaled.
      if (fds[1].revents & POLLIN) {
        uint64_t dummy;
        read(exit_event_fd, &dummy, sizeof(dummy));
        break;
      }

      // Process any inotify events.
      if (fds[0].revents & POLLIN) {
        ssize_t length = read(inotify.fd(), buffer, event_buffer_size);
        if (length < 0) {
          if (errno == EINTR) continue;
          std::cerr << "[ERROR] inotify read: " << std::strerror(errno) << "\n";
          break;
        }
        // Walk through the events.
        for (ssize_t i = 0; i < length;) {
          inotify_event *event = reinterpret_cast<inotify_event *>(&buffer[i]);
          if (event->mask & (IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_FROM |
                             IN_MOVED_TO)) {
            // Signal the build thread by writing to the build_event eventfd.
            uint64_t one = 1;
            ssize_t written = write(build_event_fd, &one, sizeof(one));
            if (written != sizeof(one)) {
              std::cerr << "[ERROR] write to build eventfd failed: "
                        << std::strerror(errno) << "\n";
            }
            // (Optionally break after one event if you want to coalesce
            // multiple events.)
          }
          i += sizeof(inotify_event) + event->len;
        }
      }
    }
    LOG_INFO("[INFO] Exiting watch_directory thread.");
  } catch (const std::exception &e) {
    LOG_ERROR("[ERROR] Exception in watch_directory: {}", e.what());
  }
}
void build_process(const std::string &build_cmd, int build_event_fd,
                   int exit_event_fd) {
  struct pollfd fds[2];
  fds[0].fd = build_event_fd;
  fds[0].events = POLLIN;
  fds[1].fd = exit_event_fd;
  fds[1].events = POLLIN;

  while (true) {
    int poll_ret = poll(fds, 2, -1);
    if (poll_ret < 0) {
      if (errno == EINTR) continue;
      LOG_ERROR("[ERROR] poll() in build_process: {}", std::strerror(errno));
      break;
    }

    if (fds[1].revents & POLLIN) {
      uint64_t dummy;
      read(exit_event_fd, &dummy, sizeof(dummy));
      break;
    }
    if (fds[0].revents & POLLIN) {
      uint64_t count;
      read(build_event_fd, &count, sizeof(count));
      LOG_INFO("[INFO] Build event received; executing build command...");
      int ret = std::system(build_cmd.c_str());
      LOG_INFO("[INFO] Build command finished with exit code: {}", ret);

      SDL_Event sdl_event;
      SDL_zero(sdl_event);
      sdl_event.type = SDL_EVENT_USER;
      sdl_event.user.code = WV_EVENT_RELOAD_MODULE;
      if (!SDL_PushEvent(&sdl_event)) {
        LOG_ERROR("[ERROR] SDL_PushEvent failed: {}", SDL_GetError());
      }
    }
  }
  LOG_INFO("[INFO] Exiting build_process thread.");
}
wv::FileWatcher::FileWatcher(const std::string &targetDir,
                             const std::string &buildCommand)
    : target_dir_(targetDir),
      build_cmd_(buildCommand),
      exitEvent_(0),
      buildEvent_(0) {}

wv::FileWatcher::~FileWatcher() {
  exitEvent_.notify(1);
  if (watcher_thread_.joinable()) {
    watcher_thread_.join();
  }
  if (build_thread_.joinable()) {
    build_thread_.join();
  }
}

void wv::FileWatcher::start() {
  watcher_thread_ = std::thread(watch_directory, target_dir_, buildEvent_.fd(),
                                exitEvent_.fd());
  build_thread_ =
      std::thread(build_process, build_cmd_, buildEvent_.fd(), exitEvent_.fd());
}
