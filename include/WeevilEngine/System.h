//
// Created by nelson on 1/17/25.
//

#ifndef WEEVIL_SRC_SYSTEM_SYSTEM_H_
#define WEEVIL_SRC_SYSTEM_SYSTEM_H_

#include "entt/entt.hpp"
#include <string>
#include <vector>

/**
 * Distinct "steps" or phases for systems to run in.
 */
enum class Stage {
  Input,
  Update,
  Render,
};

/**
 * Base interface for all systems.
 * - Each system has a stage.
 * - Each system can depend on other systems by name.
 * - Each system must implement a `run()` method.
 */
class ISystem {
 public:
  virtual ~ISystem() = default;

  /// Returns the stage in which this system runs (Input, Update, Render, etc.)
  virtual Stage stage() const = 0;

  /// Returns a list of names of systems that must run before this one.
  virtual const std::vector<std::string>& runsAfter() const = 0;

  /// Returns a unique name for this system.
  virtual const std::string& name() const = 0;

  /// The actual system logic: do whatever is needed for this step.
  virtual void run(entt::registry& registry, float dt) = 0;
};

#endif //WEEVIL_SRC_SYSTEM_SYSTEM_H_
