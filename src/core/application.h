#pragma once
#include "core/module_manager.h"
#include "pch.h"
#include "platform/common/platform.h"

namespace wv {
class Application {
 public:
  Application();
  SDL_AppResult init();
  SDL_AppResult process_event(SDL_Event& event);
  SDL_AppResult iterate();
  void shutdown();

 private:
  // Add the expected context variables in the registry
  void init_registry();
  bool quitting_ = false;
  wv::Platform platform_;
  entt::registry registry_;
  entt::dispatcher dispatcher_;
  ModuleManager module_manager_;
  wv::AppSettings settings_;
};
}  // namespace wv