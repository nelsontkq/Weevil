#pragma once

#include "asset_loader.h"
#include "module_data.h"
#include "pch.h"
#include "platform/common.h"

namespace wv {
class Module;
class ModuleManager {
 public:
  ModuleManager(entt::dispatcher& dispatcher, wv::AppSettings& settings);
  auto init() -> void;
  auto update(float deltaTime) -> void;
  auto shutdown() -> void;
  auto registry() -> entt::registry& { return registry_; }

 private:
  void load_modules();
  void init_module(ModuleData& registry);
  entt::dispatcher& dispatcher_;
  wv::AppSettings& settings_;
  wv::HotReloader hot_reloader_;
  std::unordered_map<std::string, ModuleData> modules_;
  entt::registry registry_;
};
}  // namespace wv
