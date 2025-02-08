#pragma once
#include <weevil/pch.h>
#ifdef WV_PLATFORM_LINUX
#include "internal/event_fd.h"
#endif
namespace wv {

class FileWatcher {
 public:
  FileWatcher(const std::string &targetDir, const std::string &buildCommand);
  ~FileWatcher();
  void start();

 private:
  std::string target_dir_;
  std::string build_cmd_;
#ifdef WV_PLATFORM_LINUX
  EventFd exitEvent_;
  EventFd buildEvent_;
#endif
  std::thread watcher_thread_;
  std::thread build_thread_;
};

}  // namespace wv