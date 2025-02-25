#include <spawn.h>
#include <sys/inotify.h>
#include <sys/poll.h>
#include <sys/wait.h>

#include "core/custom_events.h"
#include "event_fd.h"
#include "hot_reloader.h"
#include "pch.h"

wv::HotReloader::HotReloader() : exit_event_(0), stop_flag_(false) {}

wv::HotReloader::~HotReloader() { stop(); }

void wv::HotReloader::stop() {
  if (stop_flag_) {
    return;
  }
  stop_flag_ = true;
  exit_event_.notify(1);
  queue_cv_.notify_all();

  if (watcher_thread_.joinable()) {
    watcher_thread_.join();
  }
  if (worker_thread_.joinable()) {
    worker_thread_.join();
  }
}

void wv::HotReloader::start(const std::filesystem::path &src_dir, std::string debug_preset,
                                 entt::dispatcher *dispatcher) {
  src_dir_ = src_dir;
  debug_preset_ = debug_preset;
  dispatcher_ = dispatcher;
  watcher_thread_ = std::thread(&wv::HotReloader::watch_modules_src, this);
  worker_thread_ = std::thread(&wv::HotReloader::worker_loop, this);
}
void wv::HotReloader::worker_loop() {
  while (!stop_flag_) {
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      queue_cv_.wait(lock, [this] { return !build_queue_.empty() || stop_flag_; });
      if (stop_flag_) break;
      auto key = build_queue_.front();
      build_queue_.pop();
      int status = run_build_command(key);
      if (status == 0) {
        dispatcher_->trigger<ReloadModuleEvent>();
      } else {
        CORE_ERROR("[HotReloader] Build failed for module: {}", key);
      }
    }
  }
}

int wv::HotReloader::run_build_command(std::string &target) {
  // Construct the build command.
  std::string cmd = "cd " + src_dir_.string() + " && cmake --build --preset " + debug_preset_ + " --target " + target;
  CORE_INFO("[HotReloader] Running build command: {}", cmd);

  // Fork a new process to run the build.
  pid_t pid = fork();
  if (pid < 0) {
    CORE_ERROR("[HotReloader] fork failed: {}", std::strerror(errno));
    return errno;
  }
  if (pid == 0) {
    // Child process: execute the build command.
    execl("/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
    CORE_ERROR("[HotReloader] execl failed: {}", std::strerror(errno));
    exit(127);
  }

  int status = 0;
  while (true) {
    pid_t w = waitpid(pid, &status, 0);
    if (w == -1) {
      if (errno == EINTR) {
        continue;
      }
      CORE_ERROR("[HotReloader] waitpid failed: {}", std::strerror(errno));
      break;
    }
    break;
  }
  return status;
}
void wv::HotReloader::watch_modules_src() {
  int inotify_fd = inotify_init();
  if (inotify_fd < 0) {
    CORE_ERROR("[HotReloader] inotify_init failed: {}", std::strerror(errno));
    return;
  }
  std::unordered_map<int, std::string> wd_to_path;
  std::function<void(const std::string &)> add_watch_recursive = [&](const std::string &path) {
    int wd =
        inotify_add_watch(inotify_fd, path.c_str(), IN_CREATE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO | IN_DELETE);
    if (wd < 0) {
      CORE_ERROR("[HotReloader] inotify_add_watch failed for {}: {}", path, std::strerror(errno));
      return;
    }
    wd_to_path[wd] = path;
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
      if (entry.is_directory()) {
        add_watch_recursive(entry.path().string());
      }
    }
  };
  const std::string modules_dir = (src_dir_ / "modules").string();

  // Add a watch on the base modules directory to detect new module directories.
  int base_wd = inotify_add_watch(inotify_fd, modules_dir.c_str(), IN_CREATE | IN_MOVED_TO | IN_DELETE);
  if (base_wd < 0) {
    CORE_ERROR("[HotReloader] inotify_add_watch failed for base directory {}: {}", modules_dir,
               std::strerror(errno));
    close(inotify_fd);
    return;
  }
  wd_to_path[base_wd] = modules_dir;

  for (const auto &entry : std::filesystem::directory_iterator(modules_dir)) {
    if (entry.is_directory()) {
      add_watch_recursive(entry.path().string());
    }
  }

  // Setup polling on the inotify file descriptor and an exit event.
  constexpr size_t event_buffer_size = 1024 * (sizeof(inotify_event) + 16);
  char buffer[event_buffer_size];
  struct pollfd fds[2];
  fds[0].fd = inotify_fd;
  fds[0].events = POLLIN;
  fds[1].fd = exit_event_.fd();
  fds[1].events = POLLIN;

  while (!stop_flag_) {
    int poll_ret = poll(fds, 2, -1);
    if (poll_ret < 0) {
      if (errno == EINTR) continue;
      CORE_ERROR("[HotReloader] poll() in watch_modules_src: {}", std::strerror(errno));
      break;
    }
    if (fds[1].revents & POLLIN) {
      CORE_INFO("[HotReloader] Exiting watch_modules_src thread.");
      break;
    }
    if (fds[0].revents & POLLIN) {
      ssize_t len = read(inotify_fd, buffer, event_buffer_size);
      if (len < 0) {
        CORE_ERROR("[HotReloader] read() in watch_modules_src: {}", std::strerror(errno));
        break;
      }
      // Use a set to avoid enqueueing multiple builds for the same module in one poll cycle.
      std::unordered_set<std::string> keys_built;
      for (char *ptr = buffer; ptr < buffer + len;) {
        auto event = (inotify_event *)(ptr);
        std::string dir = wd_to_path[event->wd];
        std::string full_path = dir + "/" + event->name;
        if (dir == modules_dir && (event->mask & (IN_CREATE | IN_MOVED_TO))) {
          std::filesystem::path potential_module(full_path);
          if (std::filesystem::is_directory(potential_module)) {
            CORE_INFO("[HotReloader] New module directory detected: {}", full_path);
            add_watch_recursive(full_path);
          }
        }

        // Only process file events (ignore directories) for source files.
        if (!(event->mask & IN_ISDIR)) {
          // Compute the relative path from the base modules directory.
          auto rel_path = std::filesystem::relative(full_path, modules_dir);
          std::string module_name;
          WV_ASSERT(!rel_path.empty(), "Invalid path");
          module_name = rel_path.begin()->string();
          if (keys_built.find(module_name) == keys_built.end()) {
            CORE_INFO("[HotReloader] Change detected in module: {}", module_name);
            build_queue_.push(module_name);
            keys_built.insert(module_name);
            queue_cv_.notify_one();
          }
        }

        ptr += sizeof(inotify_event) + event->len;
      }
    }
  }

  // Cleanup: remove all watches and close the inotify file descriptor.
  for (const auto &entry : wd_to_path) {
    inotify_rm_watch(inotify_fd, entry.first);
  }
  close(inotify_fd);
  CORE_INFO("[HotReloader] Exiting watch_modules_src thread.");
}
