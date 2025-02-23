#pragma once

#include "pch.h"
#include "platform/common/rendering/renderer.h"
namespace wv {
class Platform {
 public:
  Platform();
  bool init(AppSettings& settings);
  void render(entt::registry& registry);
  void shutdown();

 private:
  SDL_Renderer* sdl_renderer_;
  SDL_Window* sdl_window_;
  wv::Renderer renderer_;
};
}  // namespace wv