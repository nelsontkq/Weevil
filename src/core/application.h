#pragma once
#include <weevil/pch.h>

#include "core/game_module.h"

namespace wv {
class AppSettings;
class FileWatcher;
class Application {
 public:
  explicit Application(const AppSettings &settings);
  void shutdown();
  SDL_AppResult process_event(SDL_Event &event);
  SDL_AppResult iterate();

 private:
  SDL_Window *sdl_window_;
  SDL_Renderer *sdl_renderer_;
  uint64_t delta_ticks_ = 0;
  GameModule game_module_;
  FileWatcher *file_watcher_;
};
}  // namespace wv