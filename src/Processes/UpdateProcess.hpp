#pragma once

#include <entt/process/process.hpp>

class UpdateProcess : public entt::process<UpdateProcess> {
public:
    UpdateProcess(entt::registry& registry)
        : registry_(registry) {}

    void update() {
        while (true) {
            // Update game logic, e.g., systems that update entity components
            // Example: movement systems, AI behavior systems, etc.

            // Yield control back to the scheduler
            co_yield;
        }
    }

private:
    entt::registry& registry_;
};
