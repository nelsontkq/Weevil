#pragma once
#include "NoCopy.h"
#include "weevil.h"
#ifdef WV_PLATFORM_LINUX
#include <sys/inotify.h>
#include <sys/poll.h>
#include <unistd.h>
#endif

namespace wv {
class FileHelper {
 public:
#ifdef WV_PLATFORM_LINUX
  static void watch_directory(const std::string& directory_path, SDL_Event ev) {
    LOG_INFO("Watching directory: {}", directory_path);
    int inotify_fd = inotify_init1(IN_NONBLOCK);
    if (inotify_fd < 0) {
      LOG_ERROR("Error initializing inotify: {}", std::strerror(errno));
      return;
    }
    int watch_descriptor = inotify_add_watch(
        inotify_fd, directory_path.c_str(),
        IN_CREATE | IN_MODIFY | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    if (watch_descriptor < 0) {
      LOG_ERROR("Error adding watch on {0}, {1}", directory_path,
                std::strerror(errno));
      close(inotify_fd);
      return;
    }
    // Define a buffer large enough to hold several events.
    constexpr size_t event_buffer_size = 1024 * (sizeof(inotify_event) + 16);
    char buffer[event_buffer_size];

    while (true) {
      struct pollfd pfd;
      pfd.fd = inotify_fd;
      pfd.events = POLLIN;
      int poll_result = poll(&pfd, 1, -1);
      if (poll_result > 0) {
        if (pfd.revents & POLLIN) {
          ssize_t length = read(inotify_fd, buffer, event_buffer_size);
          if (length < 0) {
            if (errno == EINTR) {
              continue;
            }
            LOG_ERROR("Error reading inotify events: {}", std::strerror(errno));
            break;
          }
          for (ssize_t i = 0; i < length;) {
            inotify_event* event = reinterpret_cast<inotify_event*>(&buffer[i]);
            if (event->mask & (IN_CREATE | IN_MODIFY | IN_DELETE |
                               IN_MOVED_FROM | IN_MOVED_TO)) {
              SDL_Event e = ev;
              if (!SDL_PushEvent(&e)) {
                LOG_ERROR("SDL_PushEvent failed: {}", SDL_GetError());
              }
            }

            // Move to the next event in the buffer.
            i += sizeof(inotify_event) + event->len;
          }
        }
      }
    }
    LOG_INFO("Stopping file watching.");
    close(inotify_fd);
  }
#else
  static void watch_directory(const std::string& directory_path, SDL_Event ev) {
    LOG_ERROR("File watching not implemented on this platform.");
    throw std::runtime_error("File watching not implemented on this platform.");
  }
#endif
};
}  // namespace wv