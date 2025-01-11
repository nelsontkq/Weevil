#include "Game.hpp"
#include <coroutine>

struct RenderableComponent
{
    SDL_Rect rect;
    SDL_Color color;
};

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

    // Initialize ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize ImGui SDL2 and SDL Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window_.get(), renderer_.get());
    ImGui_ImplSDLRenderer_Init(renderer_.get());
    auto entity = registry_.create();
    registry_.emplace<RenderableComponent>(entity, SDL_Rect{100, 100, 50, 50}, SDL_Color{255, 0, 0, 255});
}

Game::~Game()
{
    // Shutdown ImGui
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

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
            // Pass events to ImGui
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
        }

        // --- Attach a process for rendering ---
        scheduler_.attach([this](auto delta, void*, auto succeed, auto fail)
                          {
            // Start the Dear ImGui frame
            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            // [Optional] Show the ImGui demo window for testing
            ImGui::ShowDemoWindow();

            // [Add your custom ImGui debug tools here]

            // Rendering
            ImGui::Render();

            // Rendering
            ImGui::Render();
            SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 0, 255);
            SDL_RenderClear(renderer_.get());

            // Render entities with RenderableComponent
            registry_.view<RenderableComponent>().each([&](auto entity, RenderableComponent& renderable) {
                SDL_SetRenderDrawColor(renderer_.get(), renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
                SDL_RenderFillRect(renderer_.get(), &renderable.rect);
            });

            // Render ImGui on top of everything else
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            // Render ImGui on top of everything else
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
            SDL_RenderPresent(renderer_.get()); 
        });

        // --- Main game loop ---
        while (isRunning)
        {
            // Calculate deltaTime
            uint64_t currentTicks = SDL_GetTicks64();
            deltaTime = currentTicks - previousTicks;
            previousTicks = currentTicks;

            // Update the scheduler
            scheduler_.update(static_cast<uint32_t>(deltaTime));
        }
    }
}
