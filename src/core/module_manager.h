#pragma once

#include "module_data.h"
#include "pch.h"
#include "platform/common.h"
#include "asset_loader.h"

namespace wv {
class IModule;
class ModuleManager {
 public:
  ModuleManager(entt::dispatcher& dispatcher, wv::AppSettings& settings);
  auto init() -> void;
  auto update(float deltaTime) -> std::vector<entt::registry*>&;
  auto shutdown() -> void;

 private:
  void load_modules();
  void init_module(ModuleData& registry);
  entt::dispatcher& dispatcher_;
  wv::AppSettings& settings_;
  wv::HotReloader hot_reloader_;
  std::unordered_map<std::string, ModuleData> modules_;
  std::vector<entt::registry*> registries_;
};
}  // namespace wv
