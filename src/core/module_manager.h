#pragma once

#include <platform/common/hot_reloader.h>
#include <weevil/pch.h>

namespace wv {
class IModule;

struct ModuleData {
  SDL_SharedObject* so;
  IModule* mod;
  char* lib_path;
};
class ModuleManager {
 public:
  ModuleManager();
  void load_modules();
  void reload_module(size_t module);

  void init(wv::AppSettings& settings);
  void update(SDL_Renderer* renderer, float deltaTime);
  void process_event(SDL_Event& event);
  void shutdown();

 private:
  wv::IModule* build_module(SDL_SharedObject* so);
  std::unordered_map<size_t, ModuleData> modules_;
  wv::HotReloader hot_reloader_;
};
}  // namespace wv
