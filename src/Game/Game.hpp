#pragma once

#include <entt/entt.hpp>
#include "../Processes/UpdateProcess.hpp"
#include "../../include/SDL_Deleter.hpp"
#include "../Processes/RenderProcess.hpp"

#include <SDL2/SDL.h>
#include <memory>            // Include for smart pointers
#include "../SDL_Deleter.hpp" // Include your custom deleter

class Game
{
public:
    Game();
    ~Game();

    void run();

private:
    entt::registry registry_;
    // Add SDL window and renderer
    std::unique_ptr<SDL_Window, SDL_Deleter> window_;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
};
