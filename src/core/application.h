#pragma once
#include <weevil/pch.h>

#include "core/module_manager.h"

namespace wv {
class Application {
 public:
  Application();
  void init();
  void shutdown();
  SDL_AppResult process_event(SDL_Event &event);
  SDL_AppResult iterate();

 private:
  bool quitting_ = false;
  SDL_Window *sdl_window_;
  SDL_Renderer *sdl_renderer_;
  uint64_t delta_ticks_ = 0;
  ModuleManager module_manager_;
  wv::AppSettings settings_;
};
}  // namespace wv