#pragma once

#include <core/module_data.h>

#include "pch.h"
namespace wv {
class Platform {
 public:
  Platform();
  bool init(AppSettings& settings);
  void render(const std::unordered_map<std::string, wv::ModuleData>& registries);
  void shutdown();

 private:
  SDL_Renderer* sdl_renderer_;
  SDL_Window* sdl_window_;
};
}  // namespace wv