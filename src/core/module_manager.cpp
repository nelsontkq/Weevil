#include "module_manager.h"

#include <weevil/core/app_settings.h>
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
      auto &mod = item->second.mod;
      mod->registry.ctx().emplace<wv::Window>(settings_.width, settings_.height);
      mod->registry.ctx().emplace<Rngen>();
      mod->init(dispatcher_);
    }
  }
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

std::unordered_map<std::string, wv::ModuleData> &wv::ModuleManager::update(float deltaTime) {
  for (const auto &[key, val] : modules_) {
    val.mod->update(dispatcher_, deltaTime);
  }
  return modules_;
}
