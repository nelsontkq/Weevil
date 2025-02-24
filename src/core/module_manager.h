#pragma once

#include "module_data.h"
#include "pch.h"
#include "platform/common.h"

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
  entt::registry& registry_;
  entt::dispatcher& dispatcher_;
  wv::HotReloader hot_reloader_;
  std::unordered_map<std::string, ModuleData> modules_;
};
}  // namespace wv
