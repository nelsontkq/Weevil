// module_manager.cpp
#include "module_manager.h"

#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include <ranges>

namespace {
using GetConstCharPtr = const char *(*)();
using GetModule = wv::IModule *(*)();

}  // namespace

wv::ModuleManager::ModuleManager(){
}
void wv::ModuleManager::load_modules() {
  auto module_so_dir = std::filesystem::path(SDL_GetBasePath()) / "modules";
  auto files = std::filesystem::directory_iterator(module_so_dir);
  shutdown();

  auto ext =
#ifdef WV_PLATFORM_WINDOWS
      ".dll";
#elif defined(WV_PLATFORM_LINUX)
      ".so";
#elif defined(WV_PLATFORM_MACOS)
      ".dylib";
#endif

  for (const auto &file : files) {
    if (file.path().extension() == ext) {
      modules_.push_back(SDL_LoadObject(file.path().c_str()));
    }
  }
  for (const auto &module : modules_) {
    if (!module) {
      CORE_ERROR("Failed to load module: {}", SDL_GetError());
      continue;
    }
    auto get_module_name = (GetConstCharPtr)SDL_LoadFunction(module, "get_module_name");
    if (!get_module_name) {
      CORE_ERROR("Failed to load get_module_name: {}", SDL_GetError());
      continue;
    }
    auto create_module = (GetModule)SDL_LoadFunction(module, "create_module");
    if (!create_module) {
      CORE_ERROR("Failed to load create_module: {}", SDL_GetError());
      continue;
    }
    IModule *active_module = create_module();
    if (!active_module) {
      CORE_ERROR("Failed to create module: {}", SDL_GetError());
      continue;
    }
    std::string module_name;
    std::string relative_path = get_module_name();
    size_t pos = relative_path.find('/');
    if (pos != std::string::npos) {
      module_name = relative_path.substr(0, pos);
    } else {
      module_name = relative_path;
    }
    // Compute the key using a hash function.
    static const std::hash<std::string> hash_fn;
    size_t key = hash_fn(module_name);
    active_modules_[key] = active_module;
  }
  for (const auto &active_module : active_modules_ | std::views::values) {
    active_module->init();
  }
}
void wv::ModuleManager::shutdown() {
  for (const auto &active_module : active_modules_ | std::views::values) {
    active_module->shutdown();
    delete active_module;
  }
  for (const auto &module : modules_) {
    SDL_UnloadObject(module);
  }
  modules_.clear();
  active_modules_.clear();
}

void wv::ModuleManager::reload_module(size_t module) { hot_reloader_.rebuild_module(module); }

void wv::ModuleManager::update(SDL_Renderer *renderer, float deltaTime) {
  for (const auto &active_module : active_modules_ | std::views::values) {
    active_module->preupdate(deltaTime);
  }
  for (const auto &active_module : active_modules_ | std::views::values) {
    active_module->update(deltaTime);
  }
  for (const auto &active_module : active_modules_ | std::views::values) {
    active_module->render(renderer, deltaTime);
  }
}

void wv::ModuleManager::process_event(SDL_Event &event) {
  for (const auto &active_module : active_modules_ | std::views::values) {
    active_module->process_event(event);
  }
}
