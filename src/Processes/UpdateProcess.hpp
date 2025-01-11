#pragma once

#include <entt/process/process.hpp>

class UpdateProcess : public entt::process<UpdateProcess, Uint64> {
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
