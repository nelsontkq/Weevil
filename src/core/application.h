#pragma once
#include "core/module_manager.h"
#include "pch.h"
#include "platform/common.h"

namespace wv {
class Application {
 public:
  Application();
  SDL_AppResult init();
  SDL_AppResult process_event(SDL_Event& event);
  SDL_AppResult iterate();
  void shutdown();

 private:
  wv::AppSettings settings_;
  wv::Platform platform_;
  entt::dispatcher dispatcher_;
  ModuleManager module_manager_;
  wv::AssetLoader asset_loader_;
  bool quitting_ = false;
};
}  // namespace wv