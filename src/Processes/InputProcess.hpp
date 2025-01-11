#pragma once

#include <entt/process/process.hpp>
#include <SDL2/SDL.h>

class InputProcess : public entt::process<InputProcess> {
public:
    InputProcess(entt::registry& registry)
        : registry_(registry) {}

    void update() {
        while (true) {
            // Process input
            // Example: update entity components based on input
            co_yield;
        }
    }

private:
    entt::registry& registry_;
};
