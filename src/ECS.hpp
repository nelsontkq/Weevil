#pragma once

#include <entt/entt.hpp>
#include <functional>
#include <vector>

/**
 * An ECS "manager" that allows you to add systems for Input, Update, and Render.
 */
class ECS {
public:
    explicit ECS(entt::registry &registry)
        : registry_(registry)
    {}

    // Add an "input system"
    void add_input_system(std::function<void(entt::registry&, u_int64_t)> system) {
        inputSystems_.push_back(system);
    }

    // Add an "update system"
    void add_update_system(std::function<void(entt::registry&, u_int64_t)> system) {
        updateSystems_.push_back(system);
    }

    // Add a "render system"
    void add_render_system(std::function<void(entt::registry&, u_int64_t)> system) {
        renderSystems_.push_back(system);
    }

    // Run all input systems
    void run_input(u_int64_t dt) {
        for (auto &system : inputSystems_) {
            system(registry_, dt);
        }
    }

    // Run all update systems
    void run_update(u_int64_t dt) {
        for (auto &system : updateSystems_) {
            system(registry_, dt);
        }
    }

    // Run all render systems
    void run_render(u_int64_t dt) {
        for (auto &system : renderSystems_) {
            system(registry_, dt);
        }
    }

private:
    entt::registry &registry_;

    std::vector<std::function<void(entt::registry&, u_int64_t)>> inputSystems_;
    std::vector<std::function<void(entt::registry&, u_int64_t)>> updateSystems_;
    std::vector<std::function<void(entt::registry&, u_int64_t)>> renderSystems_;
};
