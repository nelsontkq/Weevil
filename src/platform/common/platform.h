#pragma once

#include <core/asset_loader.h>
#include <core/module_data.h>

#include "pch.h"
namespace wv {
class Platform {
 public:
  Platform();
  bool init(AppSettings& settings, wv::AssetLoader* asset_loader);
  void render(const std::vector<entt::registry*>& registries);
  void shutdown();

 private:
  SDL_Renderer* sdl_renderer_;
  SDL_Window* sdl_window_;
  wv::AssetLoader* asset_loader_;
};
}  // namespace wv