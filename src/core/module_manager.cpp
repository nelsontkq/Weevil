#include "module_manager.h"

#include <weevil/core/app_settings.h>
#include <weevil/core/components.h>
#include <weevil/core/dispatchables.h>
#include <weevil/core/rngen.h>
#include <weevil/core/window.h>

#include "pch.h"

wv::ModuleManager::ModuleManager(entt::dispatcher &dispatcher, wv::AppSettings &settings)
    : dispatcher_(dispatcher), settings_(settings) {
  registry_.ctx().emplace<wv::Window>(settings_.width, settings_.height);
  registry_.ctx().emplace<Rngen>();
}

void wv::ModuleManager::load_modules() {
  const auto module_so_dir = std::filesystem::path(SDL_GetBasePath()) / "prebuild";
  for (auto files = std::filesystem::directory_iterator(module_so_dir); const auto &file : files) {
    if (file.path().extension() != WV_DYLIB_EXT) {
      continue;
    }
    auto basename = file.path().stem().string();
    if (basename.starts_with("lib")) {
      basename = basename.substr(3);
    }
    const auto &it = modules_.find(basename);
    if (it != modules_.end()) {
      LOG_INFO("Reloading module: {}", basename);
      it->second.mod->shutdown(dispatcher_);
      dispatcher_.update();
    }
    if (const auto &[item, success] = modules_.emplace(basename, file); success) {
      init_module(item->second);
      LOG_INFO("Module loaded successfully: {}", basename);
    } else {
      LOG_ERROR("Failed to load module: {}", basename);
    }
  }
}

void wv::ModuleManager::init_module(ModuleData &module_data) {
  // might be able to create a wrapper for this so each module doesn't have to clean up
  module_data.mod->registry = &registry_;
  module_data.mod->init(dispatcher_);
}
void wv::ModuleManager::init() {
  load_modules();
  dispatcher_.sink<ReloadModuleEvent>().connect<&ModuleManager::load_modules>(*this);
  hot_reloader_.start(settings_.src_dir, settings_.debug_preset, &dispatcher_);
}
void wv::ModuleManager::shutdown() {
  hot_reloader_.stop();
  dispatcher_.sink<ReloadModuleEvent>().disconnect<&ModuleManager::load_modules>(*this);
  for (const auto &[key, val] : modules_) {
    val.mod->shutdown(dispatcher_);
  }
  modules_.clear();
}

void wv::ModuleManager::update(float deltaTime) {
  for (const auto &[key, val] : modules_) {
    val.mod->update(dispatcher_, deltaTime);
  }
}
