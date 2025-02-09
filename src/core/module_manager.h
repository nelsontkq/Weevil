#pragma once

#include <platform/common/file_watcher.h>
#include <weevil/pch.h>

class SDL_SharedObject;
namespace wv {
class AppSettings;
class IModule;

struct ModuleData {
  std::string name;
  std::string source_file;
  SDL_SharedObject* so;
};

// Loads all modules in the modules directory. Throws an exception if there’s
// not exactly 1 init module.
class ModuleManager {
 public:
  void load_modules();
  void reload_module(void* data);

  void update(SDL_Renderer* renderer, float deltaTime);

 private:
  std::unordered_map<std::string, ModuleData> modules_;
  std::unordered_map<std::string, std::unique_ptr<IModule>> active_modules_;
  wv::ModuleWatcher* file_watcher_;
};
}  // namespace wv
