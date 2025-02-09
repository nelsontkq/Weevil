#pragma once
#ifdef WV_PLATFORM_LINUX
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <sys/poll.h>
#include <unistd.h>
#include <weevil/pch.h>

#include <cstdint>
#include <stdexcept>

class InotifyWrapper {
 public:
  InotifyWrapper(const std::string &directory) {
    fd_ = inotify_init1(IN_NONBLOCK);
    if (fd_ < 0) {
      throw std::runtime_error(std::string("inotify_init1 failed: ") +
                               std::strerror(errno));
    }
    wd_ = inotify_add_watch(
        fd_, directory.c_str(),
        IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    if (wd_ < 0) {
      ::close(fd_);
      throw std::runtime_error(std::string("inotify_add_watch failed: ") +
                               std::strerror(errno));
    }
    std::cout << "[INFO] Watching directory: " << directory << "\n";
  }
  ~InotifyWrapper() {
    if (fd_ >= 0) ::close(fd_);
  }
  int fd() const { return fd_; }

 private:
  int fd_{-1};
  int wd_{-1};
};
#endif
