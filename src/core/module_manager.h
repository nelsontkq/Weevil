#pragma once

#include "internal/module_data.h"
#include "pch.h"
#include "platform/common/hot_reloader.h"
#include "platform/common/rendering/renderer.h"

namespace wv {
class IModule;

class ModuleManager {
 public:
  ModuleManager(entt::registry& registry, entt::dispatcher& dispatcher);
  auto init(AppSettings& settings) -> void;
  auto update(float deltaTime) -> void;
  auto shutdown() -> void;

 private:
  void load_modules();
  HotReloader hot_reloader_;
  entt::registry& registry_;
  entt::dispatcher& dispatcher_;
  std::unordered_map<std::string, ModuleData> modules_;
};
}  // namespace wv
