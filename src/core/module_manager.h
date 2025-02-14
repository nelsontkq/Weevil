#pragma once

#include <platform/common/hot_reloader.h>
#include <weevil/pch.h>

namespace wv {
class IModule;


// Loads all modules in the modules directory. Throws an exception if there’s
// not exactly 1 init module.
class ModuleManager {
 public:
  ModuleManager();
  void load_modules();
  void shutdown();
  void reload_module(size_t module);
  void process_event(SDL_Event& event);
  void update(SDL_Renderer* renderer, float deltaTime);

 private:
  std::vector<SDL_SharedObject*> modules_;
  std::unordered_map<size_t, IModule*> active_modules_{};
  wv::HotReloader hot_reloader_;
};
}  // namespace wv
