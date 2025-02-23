#pragma once

#include <weevil/pch.h>

namespace wv {
class Platform {
 public:
  Platform();
  bool init(AppSettings& settings);
  void shutdown();

  SDL_Window* window() { return sdl_window_; }
  SDL_Renderer* renderer() { return sdl_renderer_; }

 private:
  SDL_Renderer* sdl_renderer_;
  SDL_Window* sdl_window_;
};
}  // namespace wv