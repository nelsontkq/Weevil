#pragma once

#ifdef WV_PLATFORM_LINUX
#include <sys/eventfd.h>
#include <unistd.h>
#include <weevil/pch.h>

class EventFd {
 public:
  EventFd(uint32_t init_val = 0, int flags = 0) {
    fd_ = eventfd(init_val, flags);
    if (fd_ == -1) {
      throw std::runtime_error(std::string("eventfd creation failed: ") +
                               std::strerror(errno));
    }
  }
  ~EventFd() {
    if (fd_ != -1) {
      close(fd_);
    }
  }
  int fd() const { return fd_; }
  void notify(uint64_t value = 1) {
    ssize_t s = write(fd_, &value, sizeof(value));
    if (s != sizeof(value)) {
      CORE_ERROR("Failed to write to eventfd: {}", std::strerror(errno));
    }
  }
  // Read (and reset) the eventfd.
  uint64_t wait() {
    uint64_t value = 0;
    ssize_t s = read(fd_, &value, sizeof(value));
    if (s != sizeof(value)) {
      CORE_ERROR("Failed to read from eventfd: ", std::strerror(errno));
    }
    return value;
  }

 private:
  int fd_{-1};
};
#endif  // WV_PLATFORM_LINUX