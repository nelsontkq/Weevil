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
    ImGui_ImplSDLRenderer2_Init(renderer_.get());
    // Initialize ECS
    ecs_ = std::make_unique<ECS>(registry_, renderer_.get());

    // Register systems
    // Register render system
    ecs_->add_render_system([this](entt::registry &registry, u_int64_t dt)
                            {
        // Render entities with RenderableComponent
        registry.view<RenderableComponent>().each([this](auto /*entity*/, RenderableComponent& renderable) {
            SDL_SetRenderDrawColor(renderer_.get(), renderable.color.r, renderable.color.g, renderable.color.b, renderable.color.a);
            SDL_RenderFillRect(renderer_.get(), &renderable.rect);
        }); });

    registry_.emplace<RenderableComponent>(registry_.create(), SDL_Rect{ 100, 100, 100, 100 }, SDL_Color{ 255, 0, 0, 255 });
}

Game::~Game()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_Quit();
}

void Game::run()
{
    bool isRunning = true;
    ecs_->add_input_system([this, &isRunning](entt::registry &registry, u_int64_t dt)
                           {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
        } });

    uint64_t previousTicks = SDL_GetTicks64();
    uint64_t deltaTime = 0;
    while (isRunning)
    {
        // Calculate deltaTime
        uint64_t currentTicks = SDL_GetTicks64();
        deltaTime = currentTicks - previousTicks;
        previousTicks = currentTicks;

        // Update the scheduler
        ecs_->run_input(deltaTime);
        ecs_->run_update(deltaTime);
        ecs_->run_render(deltaTime);
    }
}
