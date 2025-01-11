#pragma once

#include <entt/entt.hpp>
#include <entt/process/process.hpp>
#include "../../include/SDL_Deleter.hpp"

#include <SDL2/SDL.h>
#include <memory>
#include "SDL_Deleter.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    entt::registry registry_;
    // Add SDL window and renderer
    entt::scheduler scheduler_;
    std::unique_ptr<SDL_Window, SDL_Deleter> window_;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
};
