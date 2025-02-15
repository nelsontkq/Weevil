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

 private:
  std::string modules_dir_;
  // might not need some of these on other platforms
#ifdef WV_PLATFORM_LINUX
  void watch_modules_src();
  void worker_loop();
  int run_build_command(size_t key);
  std::atomic_bool stop_flag_;
  EventFd exit_event_;
  std::thread watcher_thread_;
  std::thread worker_thread_;
  std::queue<size_t> build_queue_;
  std::unordered_map<size_t, std::string> modules_map_;
#else
#error "HotReloader not implemented for this platform!"
#endif
};

}  // namespace wv