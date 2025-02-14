#pragma once
#include <weevil/pch.h>
#ifdef WV_PLATFORM_LINUX
#include "internal/event_fd.h"
#endif

namespace wv {
class HotReloader {
 public:
  HotReloader();
  ~HotReloader();
  void start(const std::string &modules_dir);
  void stop();
  void rebuild_module(size_t key);

 private:
  std::string modules_dir_;
#ifdef WV_PLATFORM_LINUX
  void watch_modules_src();
  void worker_loop();
  int run_build_command(size_t key);
  EventFd exit_event_;
  std::thread watcher_thread_;

  std::vector<std::thread> worker_threads_;
  std::queue<size_t> build_queue_;
  std::mutex queue_mutex_;
  std::condition_variable queue_cv_;
  std::atomic<bool> stop_flag_;

  std::mutex modules_mutex_;
  std::unordered_map<size_t, std::string> modules_map_;

#endif
};

}  // namespace wv