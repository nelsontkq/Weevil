#pragma once
#include "SDL3/SDL.h"
#include "WeevilEngine/AppSettings.h"
#include "GameModule.h"
#include "FileWatcher.h"

namespace wv {
class Application {
 public:
  explicit Application(const wv::AppSettings &settings);
  void shutdown();
  SDL_AppResult process_event(SDL_Event &event);
  SDL_AppResult iterate();

 private:
  entt::dispatcher dispatcher_;
  SDL_Window *sdl_window_;
  SDL_Renderer *sdl_renderer_;
  uint64_t delta_ticks_ = 0;
  GameModule game_module_;
  FileWatcher* file_watcher_;
};
}  // namespace wv