#pragma once
#include <weevil/pch.h>
#ifdef WV_PLATFORM_LINUX
#include "internal/event_fd.h"
#endif

namespace wv {
class ModuleWatcher {
 public:
  ModuleWatcher(const std::string &modules_dir);
  ~ModuleWatcher();
  void start();
  void stop();

 private:
#ifdef WV_PLATFORM_LINUX
  EventFd exit_event_;
#endif
  std::thread watcher_thread_;
  std::string modules_dir_;
};

BuildScheduler scheduler_;
}  // namespace wv