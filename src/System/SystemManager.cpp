//
// Created by nelson on 1/18/25.
//

// #include <future>
#include "WeevilEngine/SystemManager.h"

#include "WeevilEngine/System.h"

namespace wv {

void SystemManager::remove_all_systems() {
  for (auto &&[id, system] : systems_) {
    system->shutdown(*registry_);
    delete system;
  }
  systems_.clear();
}
void SystemManager::remove_system(UUID name) {
  auto it = systems_.find(name);
  if (it != systems_.end()) {
    it->second->shutdown(*registry_);
    delete it->second;
    systems_.erase(it);
  }
}
void SystemManager::init() {
  for (auto &&node : systems_) {
    node.second->init(*registry_);
  }
  sort_systems();
}
void SystemManager::update(float deltaTime) {
  SystemContext ctx = {.deltaTime = deltaTime};
  for (auto &&node : sorted_systems_) {
    node.prepare(*registry_);
  }
  // TODO: Can be parallelized here
  for (auto &&node : sorted_systems_) {
    node.callback()(&ctx, *registry_);
  }
  //  std::vector<std::future<void>> tasks;
  //  for (size_t i = 0; i < sorted_systems_.size(); ++i) {
  //    const auto &node = sorted_systems_[i];
  //
  //    // If it's a top-level node or all its dependencies are completed,
  //    // we can execute it
  //    if (node.top_level()) {
  //      tasks.emplace_back(std::async(std::launch::async,
  //                                    [&node, &registry]() {
  //                                      node.callback()(node.data(), registry);
  //                                    }
  //      ));
  //    } else {
  //      // Wait for parent tasks to complete before executing this one
  //      for (auto childIdx : node.children()) {
  //        tasks[childIdx].wait();
  //      }
  //      tasks.emplace_back(std::async(std::launch::async,
  //                                    [&node, &registry]() {
  //                                      node.callback()(node.data(), registry);
  //                                    }
  //      ));
  //    }
  //  }
  //
  //  // Wait for all tasks to complete
  //  for (auto &task : tasks) {
  //    task.wait();
  //  }
}
void SystemManager::sort_systems() {
  entt::organizer organizer;
  for (auto &&[id, system] : systems_) {
    organizer.emplace<&wv::System::update>(*system);
  }
  sorted_systems_ = organizer.graph();
}
}  // namespace wv