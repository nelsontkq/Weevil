//
// Created by nelson on 1/18/25.
//

// #include <future>
#include "WeevilEngine/SystemManager.h"

#include "WeevilEngine/System.h"
#include "WeevilEngine/SystemManager.h"

namespace wv {

void SystemManager::remove_all_systems() {
  for (const auto &system : systems_ | std::views::values) {
    system->shutdown(*registry_);
    delete system;
  }
  systems_.clear();
}

void SystemManager::init(AssetManager &assets) {
  for (const auto &sys : systems_ | std::views::values) {
    sys->init(assets, *registry_);
  }
  sort_systems();
}
void SystemManager::update(float deltaTime) const {
  registry_->ctx().emplace<Time>(deltaTime);
  for (auto &&node : sorted_systems_) {
    node.prepare(*registry_);
  }
  // TODO: Can be parallelized here
  for (auto &&node : sorted_systems_) {
    auto *cb = node.callback();
    cb(node.data(), *registry_);
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
  organizer_.clear();
  for (const auto &system : systems_ | std::views::values) {
    organizer_.emplace<&wv::System::update>(*system, system->name().c_str());
  }
  sorted_systems_ = organizer_.graph();
}
}  // namespace wv