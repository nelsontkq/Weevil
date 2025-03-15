#pragma once
#include "core/module_manager.h"
#include "pch.h"
#include "platform/common.h"

namespace wv {
class Application {
 public:
  static Application* create(SDL_AppResult& out_result);
  SDL_AppResult process_event(SDL_Event& event);
  SDL_AppResult iterate();
  void shutdown();

 private:
  Application();
  SDL_AppResult init();
  entt::dispatcher dispatcher_;
  wv::AppSettings settings_;
  wv::ModuleManager module_manager_;
  wv::AssetLoader asset_loader_;
  wv::Platform platform_;
  bool quitting_{false};
};
}  // namespace wv