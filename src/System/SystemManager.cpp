//
// Created by nelson on 1/18/25.
//

// #include <future>
#include "WeevilEngine/SystemManager.h"

#include "WeevilEngine/System.h"

namespace wv {

void SystemManager::remove_all_systems() {
  for (const auto &system : systems_) {
    system->shutdown(*registry_);
    delete system;
  }
  systems_.clear();
}

SystemManager::SystemManager(entt::registry &registry) {}
void SystemManager::init(AssetManager &assets) {
  for (const auto &sys : systems_) {
    sys->init(assets, *registry_);
  }
  sort_systems();
}
void SystemManager::update(float deltaTime) {
  registry_->ctx().emplace<Time>(deltaTime);
  for (auto &&node : sorted_systems_) {
    node.prepare(*registry_);
  }
  // TODO: Can be parallelized here
  for (auto &&node : sorted_systems_) {
    auto *cb = node.callback();
    cb(node.data(), *registry_);
  }
}
void SystemManager::sort_systems() {
  organizer_.clear();
  for (const auto &system : systems_) {
    organizer_.emplace<&wv::System::update>(*system, system->name().c_str());
  }
  sorted_systems_ = organizer_.graph();
}
// Shutdown systems without removing them
void SystemManager::shutdown() {
  for (const auto &sys : systems_) {
    sys->shutdown(*registry_);
  }
}
}  // namespace wv