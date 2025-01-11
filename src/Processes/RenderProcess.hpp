#pragma once

#include <entt/entt.hpp>
#include <entt/process/process.hpp>
#include <SDL2/SDL.h>
#include "../ECS.hpp"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>


/**
 * A process that runs render systems and handles rendering.
 */
class RenderProcess : public entt::process<RenderProcess, float> {
public:
    RenderProcess(ECS &ecs, SDL_Window *window, SDL_Renderer *renderer)
        : ecs_(ecs), window_(window), renderer_(renderer)
    {}

    void update(float dt, void *data, auto succeed, auto fail) {
        // Start ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // [Optional] Show the ImGui demo window for testing
        ImGui::ShowDemoWindow();

        // Rendering
        ImGui::Render();

        // Clear the screen
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);

        // Run all render systems
        ecs_.run_render(dt);

        // Render ImGui data
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_);

        // Present
        SDL_RenderPresent(renderer_);
    }

private:
    ECS &ecs_;
    SDL_Window   *window_{nullptr};
    SDL_Renderer *renderer_{nullptr};
};
