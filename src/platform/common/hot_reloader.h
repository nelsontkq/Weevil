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
  void start(const std::filesystem::path &src_dir, std::string debug_preset);
  void stop();

 private:
  std::filesystem::path src_dir_;
  std::string debug_preset_;
  // might not need some of these on other platforms
#ifdef WV_PLATFORM_LINUX
  void watch_modules_src();
  void worker_loop();
  int run_build_command(size_t key);
  std::atomic_bool stop_flag_;
  std::condition_variable queue_cv_;
  std::mutex queue_mutex_;
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