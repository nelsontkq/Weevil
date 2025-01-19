//
// Created by nelson on 1/18/25.
//

//#include <future>
#include "WeevilEngine/SystemManager.h"
#include "WeevilEngine/System.h"

namespace wv {

UUID SystemManager::add_system(System &&sys) {
  UUID id;
  systems_[id] = sys;
  return id;
}
void SystemManager::update(entt::registry &registry, float deltaTime) {
  SystemContext ctx = {
      .deltaTime = deltaTime
  };
  for (auto &&node : sorted_systems_) {
    node.prepare(registry);
  }
  //TODO: Can be parallelized here
  for (auto &&node : sorted_systems_) {
    node.callback()(&ctx, registry);
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
  for (auto&& system : systems_) {
    organizer.emplace<&wv::System::update>(system);
  }
  sorted_systems_ = organizer.graph();
}
} // wv