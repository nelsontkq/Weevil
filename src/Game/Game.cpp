#include "Game.hpp"
#include <coroutine>

Game::Game()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    // Create SDL window using smart pointer
    window_.reset(SDL_CreateWindow("Courtly Intrigues",
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   1920, 1080,
                                   SDL_WINDOW_SHOWN));
    if (!window_)
    {
        SDL_Quit();
        throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
    }

    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (!renderer_)
    {
        SDL_Quit();
        throw std::runtime_error("Failed to create SDL Renderer: " + std::string(SDL_GetError()));
    }

    // Example: Create an entity with a RenderableComponent
    auto entity = registry_.create();
    registry_.emplace<RenderableComponent>(entity, SDL_Rect{100, 100, 50, 50}, SDL_Color{255, 0, 0, 255});
}

Game::~Game()
{
    // No need to manually destroy renderer_ or window_

    SDL_Quit();
}

void Game::run()
{
    bool isRunning = true;
    SDL_Event event;

    // Time management
    uint64_t previousTicks = SDL_GetTicks64();
    uint64_t deltaTime = 0;

    while (isRunning)
    {
        // --- Handle SDL events ---
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            // Handle other events (e.g., input) if necessary
        }

        // --- Calculate deltaTime ---
        {
            uint64_t currentTicks = SDL_GetTicks64();
            deltaTime = currentTicks - previousTicks;
            previousTicks = currentTicks;
        }
    }
}
