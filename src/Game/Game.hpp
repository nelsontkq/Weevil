#pragma once

#include <entt/entt.hpp>

#include <SDL2/SDL.h>  // Include SDL2 header

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    entt::registry registry_;
    // Add SDL window and renderer
    SDL_Window* window_;
    SDL_Renderer* renderer_;
};
