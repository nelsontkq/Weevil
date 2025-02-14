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

void wv::HotReloader::rebuild_module(size_t key) {
  {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    build_queue_.push(key);
  }
  queue_cv_.notify_one();
}

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
  for (auto &thread : worker_threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void wv::HotReloader::start(const std::string &modules_dir) {
  modules_dir_ = modules_dir;
  watcher_thread_ = std::thread(&wv::HotReloader::watch_modules_src, this);
  const int num_workers = 4;
  for (int i = 0; i < num_workers; ++i) {
    worker_threads_.emplace_back(&wv::HotReloader::worker_loop, this);
  }
}
void wv::HotReloader::worker_loop() {
  while (!stop_flag_) {
    size_t key;
    {
      std::unique_lock<std::mutex> lock(queue_mutex_);
      queue_cv_.wait(lock, [this] { return !build_queue_.empty() || stop_flag_; });
      if (stop_flag_) break;
      key = build_queue_.front();
      build_queue_.pop();
    }
    run_build_command(key);
  }
}

int wv::HotReloader::run_build_command(size_t key) {
  std::string target;
  {
    std::lock_guard<std::mutex> lock(modules_mutex_);
    auto it = modules_map_.find(key);
    WV_ASSERT(it != modules_map_.end(), "[HotReloader] Module with key not found!");
    target = it->second;
  }

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
  int wd = inotify_add_watch(inotify_fd, modules_dir_.c_str(), IN_CREATE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
  if (wd < 0) {
    CORE_ERROR("[HotReloader] inotify_add_watch failed for {}: {}", modules_dir_, std::strerror(errno));
    close(inotify_fd);
    return;
  }

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
      // Use a set to avoid duplicate build enqueues during this poll cycle.
      std::unordered_set<size_t> keys_built;
      for (char *ptr = buffer; ptr < buffer + len;) {
        inotify_event *event = reinterpret_cast<inotify_event *>(ptr);
        if (event->mask & (IN_CREATE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO)) {
          // event->name is relative to modules_dir_.
          std::string relative_path(event->name);
          // Determine the module name by taking the first path component.
          std::string module_name;
          size_t pos = relative_path.find('/');
          if (pos != std::string::npos) {
            module_name = relative_path.substr(0, pos);
          } else {
            module_name = relative_path;
          }
          // Compute the key using a hash function.
          static const std::hash<std::string> hash_fn;
          size_t key = hash_fn(module_name);

          // If this module is new, add it to our collection.
          {
            std::lock_guard<std::mutex> mod_lock(modules_mutex_);
            if (modules_map_.find(key) == modules_map_.end()) {
              CORE_INFO("[HotReloader] New module detected: {} (key: {})", module_name, key);
              modules_map_[key] = module_name;
            }
          }
          // Avoid duplicate build enqueues in the same poll cycle.
          if (keys_built.find(key) == keys_built.end()) {
            CORE_INFO("[HotReloader] Change detected in module: {} (key: {})", module_name, key);
            {
              std::lock_guard<std::mutex> lock(queue_mutex_);
              build_queue_.push(key);
            }
            queue_cv_.notify_one();
            keys_built.insert(key);
          }
        }
        ptr += sizeof(inotify_event) + event->len;
      }
    }
  }
  close(inotify_fd);
  CORE_INFO("[HotReloader] Exiting watch_modules_src thread.");
}