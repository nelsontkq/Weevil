#pragma once
#include "SDL3/SDL.h"
#include "WeevilEngine/AppSettings.h"
#include "WeevilEngine/Renderer.h"
#include "WeevilEngine/SystemManager.h"

namespace wv {
class Application {
 public:
  explicit Application(const wv::AppSettings &settings);
  void shutdown();
  auto process_event(SDL_Event &event) -> SDL_AppResult;
  void iterate();

 private:
  entt::dispatcher dispatcher_;
  SDL_Window *sdl_window_;
  SDL_Renderer *sdl_renderer_;
  wv::SystemManager systems_;
  wv::Renderer renderer_;
  uint64_t delta_ticks_ = 0;
  entt::registry registry_;
};
}  // namespace wv