#pragma once

#include "module_data.h"
#include "pch.h"
#include "platform/common.h"

namespace wv {
class IModule;
class ModuleManager {
 public:
  ModuleManager(entt::dispatcher& dispatcher, wv::AppSettings& settings);
  auto init() -> void;
  auto update(float deltaTime) -> std::unordered_map<std::string, ModuleData>&;
  auto shutdown() -> void;

 private:
  void load_modules();
  entt::dispatcher& dispatcher_;
  wv::AppSettings& settings_;
  wv::HotReloader hot_reloader_;
  std::unordered_map<std::string, ModuleData> modules_;
};
}  // namespace wv
