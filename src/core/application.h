#pragma once
#include <weevil/pch.h>

#include "core/module_manager.h"

namespace wv {
class AppSettings;
class Application {
 public:
  Application(AppSettings &settings);
  void shutdown();
  SDL_AppResult process_event(SDL_Event &event);
  SDL_AppResult iterate();

  void change_settings(AppSettings *settings);

 private:
  SDL_Window *sdl_window_;
  SDL_Renderer *sdl_renderer_;
  uint64_t delta_ticks_ = 0;
  ModuleManager module_manager_;
  AppSettings &settings_;
};
}  // namespace wv