//
// Created by nelson on 1/17/25.
//
#ifndef WEEVIL_SRC_SYSTEM_SCHEDULER_H_
#define WEEVIL_SRC_SYSTEM_SCHEDULER_H_

/**
 * The Scheduler holds one or more organizers (for update, render, etc.),
 * builds graphs, and optionally executes them in parallel.
 */
namespace wv {

class Scheduler {
 public:
  Scheduler(entt::registry &registry, SDL_Renderer *renderer)
      : registry_(registry), renderer_(renderer)
  {}

  // Example: add an "update" system via organizer
  // systemFunc must be of type void(const void*, entt::registry&)
  template<auto SystemFunc, typename... Extra>
  void addUpdateSystem(const std::string &name = {}, const void *userData = nullptr) {
    // You can also pass the Extra template parameters to override read/write modes
    updateOrganizer_.emplace<SystemFunc, Extra...>(name.c_str(), userData);
  }

  // Add a "render" system
  template<auto SystemFunc, typename... Extra>
  void addRenderSystem(const std::string &name = {}, const void *userData = nullptr) {
    renderOrganizer_.emplace<SystemFunc, Extra...>(name.c_str(), userData);
  }

  // ----------------------------------------------------------------
  // Running the update stage
  // ----------------------------------------------------------------
  void runUpdate() {
    // 1) Build the graph
    auto graph = updateOrganizer_.graph();

    // 2) Prepare the registry for each node (this ensures pools exist)
    for (auto &node : graph) {
      node.prepare(registry_);
    }
    // can topologically sort + spawn tasks in a thread pool.
    for (auto &node : graph) {
      node.callback(node.data, registry_);
    }
  }

  // ----------------------------------------------------------------
  // Running the render stage
  // ----------------------------------------------------------------
  void runRender() {
    // 1) Build the graph
    auto graph = renderOrganizer_.graph();

    // 2) Prepare
    for (auto &node : graph) {
      node.prepare(registry_);
    }

    // 3) Execute
    for (auto &node : graph) {
      node.callback(node.data, registry_);
    }
  }

 private:
  entt::registry &registry_;
  SDL_Renderer *renderer_;

  // We maintain a separate organizer for update tasks and render tasks.
  entt::organizer updateOrganizer_;
  entt::organizer renderOrganizer_;
};
}
#endif