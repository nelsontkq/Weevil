#pragma once

#include <platform/common/hot_reloader.h>
#include <weevil/pch.h>

#include <internal/module_data.h>

namespace wv {
class IModule;

class ModuleManager {
 public:
  auto init(AppSettings& settings, SDL_Renderer* renderer) -> void;
  auto update(SDL_Renderer* renderer, float deltaTime) -> void;
  auto process_system_event(SDL_Event& event) -> void;
  auto shutdown() -> void;

 private:
  void load_modules();
  SDL_Renderer* renderer_ = nullptr;
  entt::registry registry_;
  entt::dispatcher dispatcher_;
  HotReloader hot_reloader_;
  std::unordered_map<std::string, ModuleData> modules_;
};
}  // namespace wv
