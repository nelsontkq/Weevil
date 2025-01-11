#pragma once

#include <entt/entt.hpp>
#include <entt/process/process.hpp>
#include <SDL2/SDL.h>
#include "../ECS.hpp"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

/**
 * A process that handles SDL events and runs input systems.
 */
class InputProcess : public entt::process<InputProcess, float> {
public:
    InputProcess(ECS &ecs, SDL_Window *window)
        : ecs_(ecs), window_(window)
    {}

    void init() {
        // Initialization if needed
    }

    void update(float dt, void *data, auto succeed, auto fail) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // Pass events to ImGui
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT) {
                // Handle quit event if necessary
            }
            // Handle other events as needed
        }

        // Run all input systems
        ecs_.run_input(dt);
    }

private:
    ECS &ecs_;
    SDL_Window *window_{nullptr};
};
