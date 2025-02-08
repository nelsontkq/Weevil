#include <SDL3/SDL.h>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>

#include "CustomEvents.h"
#include "WeevilEngine/FileHelper.h"
#include "WeevilEngine/weevil.h"

class FileWatcher {
 public:
  /// Constructor: specify the directory to watch and the build command to
  /// execute.
  FileWatcher(const std::string &targetDir, const std::string &buildCommand)
      : target_dir_(targetDir),
        build_cmd_(buildCommand),
        is_running(false),
        is_rebuild_pending_(false) {}

  /// Destructor: stop the watcher thread and join any running build thread.
  ~FileWatcher() {
    stop();
    if (watcher_thread_.joinable()) {
      watcher_thread_.join();
    }
    if (build_thread_.joinable()) {
      build_thread_.join();
    }
  }

  /// Start the file watching thread.
  void start() {
    is_running = true;
    SDL_Event ev;
    ev.type = SDL_EVENT_USER;
    ev.user.code = WV_EVENT_REBUILD_MODULE;
    watcher_thread_ =
        std::thread(&wv::FileHelper::watch_directory, target_dir_, ev);
  }

  void stop() { is_running = false; }

  void run_build_command() {
    if (build_thread_.joinable()) {
      // If a build is already in progress, we're done.
      build_thread_.join();
      return;
    }
    build_thread_ = std::thread(&FileWatcher::build_process, this);
  }

 private:
  std::string target_dir_;
  std::string build_cmd_;
  std::atomic_bool is_running;
  std::atomic_bool is_rebuild_pending_;
  std::thread watcher_thread_;
  std::thread build_thread_;
  std::unordered_map<std::string, std::filesystem::file_time_type>
      fileTimestamps;

  void build_process() {
    LOG_INFO("[FileWatcher] Executing build command: {}", build_cmd_);

    int ret = std::system(build_cmd_.c_str());
    LOG_INFO("[FileWatcher] Build command finished with exit code: {}", ret);

    // Post a reload event to signal that the module should be reloaded.
    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_EVENT_USER;
    event.user.code = WV_EVENT_RELOAD_MODULE;
    event.user.data1 = this;
    if (!SDL_PushEvent(&event)) {
      LOG_ERROR("SDL_PushEvent (reload) failed: {}", SDL_GetError());
    }
  }

  /// Called (from the main thread) in response to a reload event.
  /// Resets the rebuildPending flag so that new rebuild events may be
  /// processed.
  void on_reload_event() { is_rebuild_pending_ = false; }
};
