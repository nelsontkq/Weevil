#pragma once

#include "wvpch.hpp"
#include "../Utils/SDL_Deleter.hpp"
#include "../ECS.hpp"

namespace wv
{

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
}