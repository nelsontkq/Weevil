// module_manager.cpp
#include "module_manager.h"

#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include <ranges>

namespace {
using GetConstCharPtr = const char *(*)();
using GetModule = wv::IModule *(*)();

}  // namespace

wv::ModuleManager::ModuleManager() {}

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

  std::hash<std::string> hash_fn;

  for (const auto &file : files) {
    if (file.path().extension() == ext) {
      // libmodule_name.so
      auto module_name = hash_fn(file.path().stem().string().substr(3));
      auto so = SDL_LoadObject(file.path().c_str());
      if (!so) {
        CORE_ERROR("Failed to load module: {}", SDL_GetError());
        continue;
      }
      modules_[module_name] = {so, nullptr, SDL_strdup(file.path().c_str())};
    }
  }
}
void wv::ModuleManager::init(wv::AppSettings &settings) {
  for (auto &[so, mod, _] : modules_ | std::views::values) {
    if (!mod) {
      mod = build_module(so);
    }
  }
  hot_reloader_.start(settings.src_dir, settings.debug_preset);
}
wv::IModule *wv::ModuleManager::build_module(SDL_SharedObject *so) {
  auto create_module = reinterpret_cast<GetModule>(SDL_LoadFunction(so, "create_module"));
  if (!create_module) {
    CORE_ERROR("Failed to load create_module function: {}", SDL_GetError());
  }
  auto *mod = create_module();
  if (!mod) {
    CORE_ERROR("Failed to load module: {}", SDL_GetError());
  }
  mod->init();
  return mod;
}
void wv::ModuleManager::shutdown() {
  for (auto &[so, mod, path] : modules_ | std::views::values) {
    if (mod) {
      mod->shutdown();
      delete mod;
    }
    SDL_UnloadObject(so);
    SDL_free(path);
  }
  modules_.clear();
}

void wv::ModuleManager::update(SDL_Renderer *renderer, float deltaTime) {
  for (const auto &mod_data : modules_ | std::views::values) {
    mod_data.mod->preupdate(deltaTime);
  }
  for (const auto &mod_data : modules_ | std::views::values) {
    mod_data.mod->update(deltaTime);
  }
  for (const auto &mod_data : modules_ | std::views::values) {
    mod_data.mod->render(renderer, deltaTime);
  }
}

void wv::ModuleManager::process_event(SDL_Event &event) {
  for (const auto &mod_data : modules_ | std::views::values) {
    mod_data.mod->process_event(event);
  }
}

void wv::ModuleManager::reload_module(size_t module) {
  auto it = modules_.find(module);
  if (it != modules_.end()) {
    auto [so, mod, p] = it->second;
    if (mod) {
      mod->shutdown();
      delete mod;
    }
    SDL_UnloadObject(so);

    so = SDL_LoadObject(p);
    if (!so) {
      CORE_ERROR("Failed to load module: {}", SDL_GetError());
      return;
    }
    it->second = {so, build_module(so), p};
  }
}
