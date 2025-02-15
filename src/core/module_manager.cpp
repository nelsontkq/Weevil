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
      modules_[module_name] = {so, nullptr};
    }
  }
}
void wv::ModuleManager::init(std::string &file_watch_dir) {
  for (auto &[so, mod] : modules_ | std::views::values) {
    if (!mod) {
      auto create_module = reinterpret_cast<GetModule>(SDL_LoadFunction(so, "create_module"));
      if (!create_module) {
        CORE_ERROR("Failed to load create_module function: {}", SDL_GetError());
        continue;
      }
      mod = create_module();
      if (!mod) {
        CORE_ERROR("Failed to load module: {}", SDL_GetError());
        continue;
      }
      mod->init();
    }
  }
  hot_reloader_.start(file_watch_dir);
}
void wv::ModuleManager::shutdown() {
  for (const auto &[so, mod] : modules_ | std::views::values) {
    if (mod) {
      mod->shutdown();
      delete mod;
    }
    SDL_UnloadObject(so);
  }
  modules_.clear();
}

void wv::ModuleManager::update(SDL_Renderer *renderer, float deltaTime) {
  for (const auto &[so, mod] : modules_ | std::views::values) {
    mod->preupdate(deltaTime);
  }
  for (const auto &[so, mod] : modules_ | std::views::values) {
    mod->update(deltaTime);
  }
  for (const auto &[so, mod] : modules_ | std::views::values) {
    mod->render(renderer, deltaTime);
  }
}

void wv::ModuleManager::process_event(SDL_Event &event) {
  for (const auto &[_, mod] : modules_ | std::views::values) {
    mod->process_event(event);
  }
}
