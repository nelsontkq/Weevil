// module_manager.cpp
#include "module_manager.h"

#include <SDL.h>
#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include <filesystem>

// Function pointer typedefs.
namespace {
using GetConstCharPtr = const char *(*)();
using GetInt = int (*)();
using GetModule = wv::IModule *(*)();

}  // namespace

void wv::ModuleManager::load_modules() {
  std::filesystem::path module_so_dir =
      std::filesystem::path(SDL_GetBasePath()) / "modules";
  auto files = std::filesystem::directory_iterator(module_so_dir);

  active_modules_.clear();
  modules_.clear();
  for (const auto &file : files) {
    if (file.is_regular_file()) {
      auto path = file.path();
      auto so = SDL_LoadObject(path.c_str());
      if (!so) {
        CORE_ERROR("Failed to load module {}: {}", path.c_str(),
                   SDL_GetError());
        continue;
      }
      auto source_fn = (GetConstCharPtr)SDL_LoadFunction(so, "get_file_name");
      std::string source_file = source_fn();
      modules_[name] = ModuleData{name, source_file, so};
      auto create_module = (GetModule)SDL_LoadFunction(so, "create_module");
      if (create_module) {
        active_modules_.insert(std::unique_ptr<IModule>(create_module()));
      } else {
        CORE_ERROR("Failed to load create_module function from module {}",
                   name);
      }
      CORE_INFO("Module {} loaded from {}", name, path.c_str());
    }
  }
  CORE_INFO("Loaded {} modules.", modules_.size());
#if WV_HOT_RELOAD
  file_watcher_ = new wv::ModuleWatcher(settings.modules_src_dir);

  file_watcher_->start();
#endif

  for (const auto &active_module : active_modules_) {
    active_module->init();
  }
  return settings;
}

void wv::ModuleManager::reload_module(std::string &name) {
  auto it = modules_.find(name);
  if (it == modules_.end()) {
    CORE_ERROR("Module {} not found.", name);
    return;
  }
  auto &module = it->second;
  SDL_UnloadObject(module.so);
  auto so = SDL_LoadObject(module.module_path.c_str());
  if (!so) {
    CORE_ERROR("Failed to reload module {}: {}", name, SDL_GetError());
    return;
  }
  module.so = (SDL_SharedObject *)so;
  CORE_INFO("Module {} reloaded.", name);

  auto create_module = (GetModule)SDL_LoadFunction(so, "create_module");
  WV_ASSERT(create_module,
            "Failed to load create_module function from module.");
  auto &prev_module = active_modules_[name];
  prev_module->shutdown();
  auto new_module = create_module();
  new_module->init();
  prev_module = std::unique_ptr<IModule>(new_module);
  CORE_INFO("Module {} reinitialized.", name);
}
