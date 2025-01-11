#pragma once

#include <memory>
#include "../Utils/SDL_Deleter.hpp"
#include "../ECS.hpp"

#include <SDL2/SDL.h>
#include <imgui.h>
#include <entt/entt.hpp>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    entt::registry registry_;
    // Add SDL window and renderer
    entt::basic_scheduler<uint64_t> scheduler_;
    std::unique_ptr<SDL_Window, SDL_Deleter> window_;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
    std::unique_ptr<ECS> ecs_;
};
