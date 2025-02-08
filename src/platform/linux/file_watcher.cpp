#include <internal/custom_events.h>
#include <internal/event_fd.h>
#include <internal/inotify_wrapper.h>
#include <platform/common/filewatcher.h>
#include <spawn.h>
#include <sys/wait.h>

#include <weevil/pch.h>
namespace {
int run_build_command(const std::string &cmd) {
  pid_t pid = vfork();
  if (pid < 0) {
    // vfork() faciled
    std::cerr << "[ERROR] vfork failed: " << std::strerror(errno) << "\n";
    return errno;
  }

  if (pid == 0) {
    execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
    // If execl returns, an error occurred. Use _exit() to avoid flushing stdio
    _exit(127);
  }
  int status = 0;
  while (true) {
    pid_t w = waitpid(pid, &status, 0);
    if (w == -1) {
      if (errno == EINTR) continue;
      std::cerr << "[ERROR] waitpid failed: " << std::strerror(errno) << "\n";
      break;
    }
    break;
  }
  return status;
}
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
        LOG_ERROR("[ERROR] poll() in watch_directory: ", std::strerror(errno));
        break;
      }
      if (fds[1].revents & POLLIN) {
        uint64_t dummy;
        read(exit_event_fd, &dummy, sizeof(dummy));
        break;
      }

      if (fds[0].revents & POLLIN) {
        ssize_t length = read(inotify.fd(), buffer, event_buffer_size);
        if (length < 0) {
          if (errno == EINTR) continue;
          LOG_ERROR("[ERROR] inotify read: {}", std::strerror(errno));
          break;
        } else if (length == 0) {
          break;
        }
        for (ssize_t i = 0; i < length;) {
          inotify_event *event = reinterpret_cast<inotify_event *>(&buffer[i]);
          if (event->mask & (IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_FROM |
                             IN_MOVED_TO)) {
            uint64_t one = 1;
            ssize_t written = write(build_event_fd, &one, sizeof(one));
            if (written != sizeof(one)) {
              LOG_ERROR("[ERROR] write to build eventfd failed: {}",
                        std::strerror(errno));
            }
            break;
          }
          i += sizeof(inotify_event) + event->len;
        }
      }
    }
    CORE_INFO("[INFO] Exiting watch_directory thread.");
  } catch (const std::exception &e) {
    CORE_ERROR("[ERROR] Exception in watch_directory: {}", e.what());
  }
}
void build_process(const std::string &build_cmd, int build_event_fd,
                   int exit_event_fd) {
  struct pollfd fds[2];
  fds[0].fd = build_event_fd;
  fds[0].events = POLLIN;
  fds[1].fd = exit_event_fd;
  fds[1].events = POLLIN;

  auto last_called = std::chrono::high_resolution_clock::now();
  while (true) {
    int poll_ret = poll(fds, 2, -1);
    if (poll_ret < 0) {
      if (errno == EINTR) continue;
      CORE_ERROR("[ERROR] poll() in build_process: {}", std::strerror(errno));
      break;
    }

    if (fds[1].revents & POLLIN) {
      uint64_t dummy;
      read(exit_event_fd, &dummy, sizeof(dummy));
      break;
    }
    if (fds[0].revents & POLLIN) {
      uint64_t count;
      auto res = read(build_event_fd, &count, sizeof(count));
      if (last_called + std::chrono::milliseconds(500) >
          std::chrono::high_resolution_clock::now()) {
        continue;
      }
      if (res == 0) {
        break;
      } else if (res < 0) {
        if (errno == EINTR) continue;
        CORE_ERROR("[ERROR] read from build eventfd failed: {}",
                   std::strerror(errno));
        break;
      }

      // log exact time for benchmarking
      auto start = std::chrono::high_resolution_clock::now();
      int ret = run_build_command(build_cmd);
      last_called = std::chrono::high_resolution_clock::now();
      CORE_INFO("[INFO] Build time: {} ms",
                std::chrono::duration_cast<std::chrono::milliseconds>(
                    last_called - start)
                    .count());

      SDL_Event sdl_event;
      SDL_zero(sdl_event);
      sdl_event.type = SDL_EVENT_USER;
      sdl_event.user.code = WV_EVENT_RELOAD_MODULE;
      if (!SDL_PushEvent(&sdl_event)) {
        CORE_ERROR("[ERROR] SDL_PushEvent failed: {}", SDL_GetError());
      }
    }
  }
  CORE_INFO("[INFO] Exiting build_process thread.");
}

}  // namespace
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
