// module_manager.cpp
#include "module_manager.h"

#include <weevil/core/app_settings.h>
#include <weevil/core/rngen.h>
#include <weevil/core/window.h>

#include "pch.h"
wv::ModuleManager::ModuleManager(entt::registry &registry, entt::dispatcher &dispatcher)
    : registry_(registry), dispatcher_(dispatcher) {}

void wv::ModuleManager::load_modules() {
  auto module_so_dir = std::filesystem::path(SDL_GetBasePath()) / "prebuild";
  auto files = std::filesystem::directory_iterator(module_so_dir);

  for (const auto &file : files) {
    // cross platform check if file is a shared object
    if (file.path().extension() != WV_DYLIB_EXT) {
      continue;
    }
    auto basename = file.path().stem().string();
    if (basename.starts_with("lib")) {
      basename = basename.substr(3);
    }
    auto mod_data = modules_.find(basename);
    if (mod_data != modules_.end()) {
      mod_data->second.mod->shutdown(registry_, dispatcher_);
    }
    auto mod = modules_.insert_or_assign(basename, ModuleData(file));
    mod.first->second.mod->init(registry_, dispatcher_);
  }
}
void wv::ModuleManager::init(AppSettings &settings) {
  load_modules();
  dispatcher_.sink<ReloadModuleEvent>().connect<&ModuleManager::load_modules>(*this);
  hot_reloader_.start(settings.src_dir, settings.debug_preset, &dispatcher_);
}
void wv::ModuleManager::shutdown() {
  hot_reloader_.stop();
  dispatcher_.sink<ReloadModuleEvent>().disconnect<&ModuleManager::load_modules>(*this);
  for (auto &mod_data : modules_ | std::views::values) {
    mod_data.mod->shutdown(registry_, dispatcher_);
  }
  modules_.clear();
}

void wv::ModuleManager::update(float deltaTime) {
  for (const auto &mod_data : modules_ | std::views::values) {
    mod_data.mod->update(registry_, dispatcher_, deltaTime);
  }
}
