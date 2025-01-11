#include "Game.hpp"

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
                                   800, 600,
                                   SDL_WINDOW_SHOWN));
    if (!window_)
    {
        SDL_Quit();
        throw std::runtime_error("Failed to create SDL Window: " + std::string(SDL_GetError()));
    }

    // Create SDL renderer using smart pointer
    renderer_.reset(SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED));
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
    // Clean up SDL resources
    // No need to manually destroy renderer_ or window_
    SDL_Quit();
}

void Game::run()
{
    bool isRunning = true;
    SDL_Event event;

    // Create EnTT process scheduler
    entt::scheduler scheduler;

    // Create and attach processes
    auto &updateProcess = scheduler.attach<UpdateProcess>(registry_);
    auto &renderProcess = scheduler.attach<RenderProcess>(registry_, renderer_.get());
    // You can also attach InputProcess if needed

    // Time management
    Uint32 frameStart = 0;
    float deltaTime = 0.0f;

    while (isRunning)
    {
        Uint32 currentTicks = SDL_GetTicks();
        deltaTime = (currentTicks - frameStart) / 1000.0f; // Convert milliseconds to seconds
        frameStart = currentTicks;

        // Handle SDL events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            // Handle other events if necessary
        }

        // Update EnTT processes with the calculated deltaTime
        scheduler.update(deltaTime);

        // Frame rate control
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}
