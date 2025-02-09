#include <internal/custom_events.h>
#include <internal/event_fd.h>
#include <platform/common/file_watcher.h>
#include <spawn.h>
#include <sys/inotify.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <weevil/pch.h>

namespace {
int run_build_command(const std::string &cmd) {
  pid_t pid = vfork();
  if (pid < 0) {
    CORE_ERROR("[ModuleWatcher] vfork failed: {}", std::strerror(errno));
    return errno;
  }
  if (pid == 0) {
    execl("/usr/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
    _exit(127);
  }
  int status = 0;
  while (true) {
    pid_t w = waitpid(pid, &status, 0);
    if (w == -1) {
      if (errno == EINTR) continue;
      std::cerr << "[ModuleWatcher] waitpid failed: " << std::strerror(errno)
                << "\n";
      break;
    }
    break;
  }
  return status;
}

void watch_modules_src(const std::string &modules_src_dir, int exit_event_fd) {
  int inotify_fd = inotify_init();
  if (inotify_fd < 0) {
    CORE_ERROR("[ModuleWatcher] inotify_init failed: {}", std::strerror(errno));
    return;
  }
  int wd =
      inotify_add_watch(inotify_fd, modules_src_dir.c_str(),
                        IN_CREATE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
  if (wd < 0) {
    CORE_ERROR("[ModuleWatcher] inotify_add_watch failed for {}: {}",
               modules_src_dir, std::strerror(errno));
    close(inotify_fd);
    return;
  }

  constexpr size_t event_buffer_size = 1024 * (sizeof(inotify_event) + 16);
  char buffer[event_buffer_size];
  struct pollfd fds[2];
  fds[0].fd = inotify_fd;
  fds[0].events = POLLIN;
  fds[1].fd = exit_event_fd;
  fds[1].events = POLLIN;

  while (true) {
    int poll_ret = poll(fds, 2, -1);
    if (poll_ret < 0) {
      if (errno == EINTR) continue;
      CORE_ERROR("[ModuleWatcher] poll() in watch_modules_src: {}",
                 std::strerror(errno));
      break;
    }
    if (fds[1].revents & POLLIN) {
      CORE_INFO("[ModuleWatcher] Exiting watch_modules_src thread.");
      break;
    }
    if (fds[0].revents & POLLIN) {
      ssize_t len = read(inotify_fd, buffer, event_buffer_size);
      if (len < 0) {
        CORE_ERROR("[ModuleWatcher] read() in watch_modules_src: {}",
                   std::strerror(errno));
        break;
      }
      for (char *ptr = buffer; ptr < buffer + len;) {
        inotify_event *event = (inotify_event *)ptr;
        // if written, deleted, or modified, rebuild
        if (event->mask &
            (IN_CREATE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO)) {
          auto file_path = std::string(event->name);
          // base dir
          file_path = file_path.substr(modules_src_dir.size() - 1);
          file_path = file_path.substr(0, file_path.find_first_of('/'));
          CORE_INFO("[ModuleWatcher] File {} changed.", file_path);
          SDL_Event event;
          SDL_zero(event);
          event.type = SDL_EVENT_USER;
          event.user.code = wv::EngineEvent::WV_EVENT_RELOAD_MODULE;
          event.user.data1 = SDL_strdup(file_path.c_str());
          SDL_PushEvent(&event);
        }
      }
    }
    CORE_INFO("[ModuleWatcher] Exiting watch_modules_src thread.");
    close(inotify_fd);
  }

}  // namespace

wv::ModuleWatcher::ModuleWatcher(const std::string &modules_dir) : exit_event_(0) {}

wv::ModuleWatcher::~ModuleWatcher() {
  exit_event_.notify(1);
  if (watcher_thread_.joinable()) {
    watcher_thread_.join();
  }
}

void wv::ModuleWatcher::stop() {
  exit_event_.notify(1);
  if (watcher_thread_.joinable()) {
    watcher_thread_.join();
  }
}

void wv::ModuleWatcher::start() {
  watcher_thread_ = std::thread(watch_modules_src, modules_dir_, exit_event_);
}
