#pragma once

#include <entt/process/process.hpp>
#include <SDL2/SDL.h>

class InputProcess : public entt::process<InputProcess, Uint64> {
public:
    InputProcess(entt::registry& registry)
        : registry_(registry) {}

    void update(float deltaTime) {
        // Process input
        // Example: update entity components based on input
    }

private:
    entt::registry& registry_;
};
