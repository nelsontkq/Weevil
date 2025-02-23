#pragma once
#include <internal/custom_events.h>
#include "pch.h"
#ifdef WV_PLATFORM_LINUX
#include "internal/event_fd.h"
#endif

namespace wv {
class HotReloader {
 public:
  HotReloader();

  ~HotReloader();
  void start(const std::filesystem::path& src_dir, std::string debug_preset, entt::dispatcher* dispatcher);
  void stop();

 private:
  std::filesystem::path src_dir_;
  std::string debug_preset_;
  // might not need some of these on other platforms
#ifdef WV_PLATFORM_LINUX
  void watch_modules_src();
  void worker_loop();
  auto run_build_command(std::string& target) -> int;
  std::condition_variable queue_cv_;
  std::mutex queue_mutex_;
  EventFd exit_event_;
  std::atomic_bool stop_flag_;
  std::thread watcher_thread_;
  std::thread worker_thread_;
  std::queue<std::string> build_queue_;
#else
#error "HotReloader not implemented for this platform!"
#endif
  entt::dispatcher* dispatcher_;
};

}  // namespace wv
