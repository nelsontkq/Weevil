#pragma once

#include <entt/process.hpp>
#include <SDL2/SDL.h>

class InputProcess : public entt::process<InputProcess, float> {
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
