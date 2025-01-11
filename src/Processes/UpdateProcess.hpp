#pragma once

#include <entt/process.hpp>

class UpdateProcess : public entt::process<UpdateProcess, float> {
public:
    UpdateProcess(entt::registry& registry)
        : registry_(registry) {}

    void update(float deltaTime) {
        // Update game logic, e.g., systems that update entity components
        // Example: movement systems, AI behavior systems, etc.
    }

private:
    entt::registry& registry_;
};
