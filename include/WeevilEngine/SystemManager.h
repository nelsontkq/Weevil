//
// Created by nelson on 1/17/25.
//

#ifndef WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_
#define WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#include "System.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stdexcept>

/**
 * Manages systems, sorts them by dependency, then runs them
 * in stage order (Input, Update, Render).
 */
class SystemManager {
 public:
  /// Add a system to the manager.
  void addSystem(std::unique_ptr<ISystem> system) {
    const std::string& sysName = system->name();
    if (systemsByName_.count(sysName) > 0) {
      throw std::runtime_error("System with name '" + sysName + "' already exists!");
    }
    systemsByName_[sysName] = system.get();
    systems_.push_back(std::move(system));

    needsSorting_ = true;
  }

  /// Run all systems in a single frame, grouped by stage.
  void runAll(entt::registry& registry, float dt) {
    if (needsSorting_) {
      sortSystemsByDependencies();
      needsSorting_ = false;
    }

    // We'll define a manual list of stages in the order we want to run them.
    // If you have more stages, extend this array.
    const Stage stageOrder[] = { Stage::Input, Stage::Update, Stage::Render };

    // For each stage in the pipeline:
    for (auto stage : stageOrder) {
      // Run only the systems that belong to this stage, in topological order.
      for (ISystem* sys : sortedSystems_) {
        if (sys->stage() == stage) {
          sys->run(registry, dt);
        }
      }
    }
  }

 private:
  /**
   * Sorts systems by dependencies (runsAfter) using topological sort (Kahn's algorithm).
   * The result is stored in `sortedSystems_`.
   */
  void sortSystemsByDependencies() {
    // Create an adjacency list of system->children
    // We'll invert "runsAfter" so we have edges: dependency -> systemThatRunsAfter
    std::unordered_map<std::string, std::vector<std::string>> edges;

    // Initialize all systems in the map
    for (auto &sysPtr : systems_) {
      edges[sysPtr->name()]; // ensures key exists with empty vector
    }

    // For each system S, for each "dep" in S->runsAfter(),
    // add an edge dep -> S
    for (auto &sysPtr : systems_) {
      for (const auto &depName : sysPtr->runsAfter()) {
        if (systemsByName_.count(depName) == 0) {
          throw std::runtime_error(
              "System '" + sysPtr->name() +
                  "' depends on unknown system '" + depName + "'"
          );
        }
        edges[depName].push_back(sysPtr->name());
      }
    }

    // Compute in-degrees (number of incoming edges)
    std::unordered_map<std::string, int> inDegree;
    for (auto &pair : edges) {
      inDegree[pair.first] = 0;
    }
    for (auto &pair : edges) {
      for (auto &child : pair.second) {
        inDegree[child]++;
      }
    }

    // Collect all nodes with inDegree == 0
    std::queue<std::string> zeroInDegree;
    for (auto &pair : inDegree) {
      if (pair.second == 0) {
        zeroInDegree.push(pair.first);
      }
    }

    // Kahn's algorithm to produce a topological order
    std::vector<ISystem*> topoResult;
    topoResult.reserve(systems_.size());

    while (!zeroInDegree.empty()) {
      auto node = zeroInDegree.front();
      zeroInDegree.pop();

      topoResult.push_back(systemsByName_[node]);

      // For each child of node, decrement its inDegree
      for (auto &childName : edges[node]) {
        if (--inDegree[childName] == 0) {
          zeroInDegree.push(childName);
        }
      }
    }

    // If we didn't include all systems, there's a cycle
    if (topoResult.size() != systems_.size()) {
      throw std::runtime_error("Cycle detected in system dependencies!");
    }

    // Store the final sorted list
    sortedSystems_ = std::move(topoResult);
  }

 private:
  // All systems, owned by the manager
  std::vector<std::unique_ptr<ISystem>> systems_;

  // Quick lookup by system name
  std::unordered_map<std::string, ISystem*> systemsByName_;

  // Final sorted list of systems (by dependency), for stable iteration
  std::vector<ISystem*> sortedSystems_;

  // Flag so we only re-sort if something changed
  bool needsSorting_ = false;
};

#endif // SYSTEMMANAGER_HPP
