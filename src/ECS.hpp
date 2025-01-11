#pragma once

#include <SDL2/SDL.h>
#include <imgui.h>
#include <entt/entt.hpp>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <functional>
#include <vector>

/**
 * An ECS "manager" that allows you to add systems for Input, Update, and Render.
 */
class ECS
{
public:
    explicit ECS(entt::registry &registry, SDL_Renderer *renderer)
        : registry_(registry), renderer_(renderer)
    {
    }

    // Add an "input system"
    void add_input_system(std::function<void(entt::registry &, uint64_t)> system)
    {
        inputSystems_.push_back(system);
    }

    // Add an "update system"
    void add_update_system(std::function<void(entt::registry &, uint64_t)> system)
    {
        updateSystems_.push_back(system);
    }

    // Add a "render system"
    void add_render_system(std::function<void(entt::registry &, uint64_t)> system)
    {
        renderSystems_.push_back(system);
    }

    // Run all input systems
    void run_input(uint64_t dt)
    {
        for (auto &system : inputSystems_)
        {
            system(registry_, dt);
        }
    }

    // Run all update systems
    void run_update(uint64_t dt)
    {
        for (auto &system : updateSystems_)
        {
            system(registry_, dt);
        }
    }

    // Run all render systems
    void run_render(uint64_t dt)
    {

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
        for (auto &system : renderSystems_)
        {
            system(registry_, dt);
        }

        // Render ImGui data
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer_);

        // Present
        SDL_RenderPresent(renderer_);
    }

private:
    entt::registry &registry_;
    SDL_Renderer *renderer_;

    std::vector<std::function<void(entt::registry &, u_int64_t)>> inputSystems_;
    std::vector<std::function<void(entt::registry &, u_int64_t)>> updateSystems_;
    std::vector<std::function<void(entt::registry &, u_int64_t)>> renderSystems_;
};
