#include "module_manager.h"

#include <weevil/core/app_settings.h>
#include <weevil/core/components.h>
#include <weevil/core/dispatchables.h>
#include <weevil/core/rngen.h>
#include <weevil/core/window.h>

#include "pch.h"

wv::ModuleManager::ModuleManager(entt::dispatcher &dispatcher, wv::AppSettings &settings)
    : dispatcher_(dispatcher), settings_(settings) {}

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
    const auto &data = modules_.find(basename);
    if (data != modules_.end()) {
      data->second.mod->shutdown(dispatcher_);
      modules_.erase(data);
    }
    if (const auto &[item, success] = modules_.emplace(basename, file); success) {
      init_module(item->second);
    } else {
      LOG_ERROR("Failed to load module: {}", basename);
    }
  }
  registries_.resize(modules_.size());
  std::transform(modules_.begin(), modules_.end(), registries_.begin(),
                 [](const auto &item) -> entt::registry * { return &item.second.mod->registry; });
}

void wv::ModuleManager::init_module(ModuleData &module_data) {
  module_data.mod->registry.ctx().emplace<wv::Window>(settings_.width, settings_.height);
  module_data.mod->registry.ctx().emplace<Rngen>();
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

std::vector<entt::registry *> &wv::ModuleManager::update(float deltaTime) {
  for (const auto &[key, val] : modules_) {
    val.mod->update(dispatcher_, deltaTime);
  }
  return registries_;
}
