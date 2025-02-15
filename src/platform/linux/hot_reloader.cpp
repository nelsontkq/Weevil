#include <internal/custom_events.h>
#include <internal/event_fd.h>
#include <platform/common/hot_reloader.h>
#include <spawn.h>
#include <sys/inotify.h>
#include <sys/poll.h>
#include <sys/wait.h>
#include <weevil/pch.h>

wv::HotReloader::HotReloader() : exit_event_(0), stop_flag_(false) {}

wv::HotReloader::~HotReloader() { stop(); }

void wv::HotReloader::stop() {
  if (stop_flag_) {
    return;
  }
  stop_flag_ = true;
  exit_event_.notify(1);

  if (watcher_thread_.joinable()) {
    watcher_thread_.join();
  }
  if (worker_thread_.joinable()) {
    worker_thread_.join();
  }
}

void wv::HotReloader::start(const std::string &modules_dir) {
  modules_dir_ = modules_dir;
  watcher_thread_ = std::thread(&wv::HotReloader::watch_modules_src, this);
  worker_thread_ = std::thread(&wv::HotReloader::worker_loop, this);
}
void wv::HotReloader::worker_loop() {
  
}

int wv::HotReloader::run_build_command(size_t key) {
  std::string target;

  auto it = modules_map_.find(key);
  WV_ASSERT(it != modules_map_.end(), "[HotReloader] Module with key not found!");
  target = it->second;

  // Construct the build command.
  std::string cmd = "cmake --build . --target " + target;
  CORE_INFO("[HotReloader] Running build command: {}", cmd);

  // Fork a new process to run the build.
  pid_t pid = fork();
  if (pid < 0) {
    CORE_ERROR("[HotReloader] fork failed: {}", std::strerror(errno));
    return errno;
  }
  if (pid == 0) {
    // In the child process, execute the build command.
    execl("/usr/bin/sh", "sh", "-c", cmd.c_str(), nullptr);
    _exit(127);  // If execl fails.
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

  // Add a watch on the base modules directory to detect new module directories.
  int base_wd = inotify_add_watch(inotify_fd, modules_dir_.c_str(), IN_CREATE | IN_MOVED_TO | IN_DELETE);
  if (base_wd < 0) {
    CORE_ERROR("[HotReloader] inotify_add_watch failed for base directory {}: {}", modules_dir_, std::strerror(errno));
    close(inotify_fd);
    return;
  }
  wd_to_path[base_wd] = modules_dir_;

  // For each subdirectory in the base modules directory (i.e. each module), add recursive watches.
  for (const auto &entry : std::filesystem::directory_iterator(modules_dir_)) {
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
      std::unordered_set<size_t> keys_built;
      for (char *ptr = buffer; ptr < buffer + len;) {
        inotify_event *event = reinterpret_cast<inotify_event *>(ptr);
        std::string dir = wd_to_path[event->wd];
        std::string full_path = dir + "/" + event->name;

        // If the event occurred in the base modules directory, check for a new module.
        if (dir == modules_dir_ && (event->mask & (IN_CREATE | IN_MOVED_TO))) {
          std::filesystem::path potential_module(full_path);
          if (std::filesystem::is_directory(potential_module)) {
            CORE_INFO("[HotReloader] New module directory detected: {}", full_path);
            add_watch_recursive(full_path);
          }
        }

        // Only process file events (ignore directories) for source files.
        if (!(event->mask & IN_ISDIR)) {
          std::string file_name(event->name);
          // Compute the relative path from the base modules directory.
          std::filesystem::path rel_path = std::filesystem::relative(full_path, modules_dir_);
          std::string module_name;
          if (!rel_path.empty()) {
            // The module name is the first component of the relative path.
            module_name = rel_path.begin()->string();
          } else {
            module_name = file_name;
          }

          // Use a hash of the module name as a key.
          static const std::hash<std::string> hash_fn;
          size_t key = hash_fn(module_name);

          if (modules_map_.find(key) == modules_map_.end()) {
            CORE_INFO("[HotReloader] New module detected: {} (key: {})", module_name, key);
            modules_map_[key] = module_name;
          }
          if (keys_built.find(key) == keys_built.end()) {
            CORE_INFO("[HotReloader] Change detected in module: {} (key: {})", module_name, key);
            build_queue_.push(key);
            keys_built.insert(key);
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
