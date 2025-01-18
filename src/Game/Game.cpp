#include <coroutine>
#include "Game.hpp"

bool wv::Game::logInitialized_ = Game::initializeLog();

auto wv::Game::initializeLog() -> bool {
  if (Game::logInitialized_) {
    return false;
  }
  Log::Init();
  return true;
}

wv::Game::Game() : settings_(AppSettings::load()), sdlContext_(settings_) {
  LOG_INFO("Initializing Game");
  // Initialize ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  // Initialize ImGui SDL2 and SDL Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(sdlContext_.get_window(), sdlContext_.get_renderer());
  ImGui_ImplSDLRenderer2_Init(sdlContext_.get_renderer());
  // Initialize ECS
  ecs_ = std::make_unique<ECS>(registry_, sdlContext_.get_renderer());
}

wv::Game::~Game() {
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void wv::Game::run() {
  LOG_INFO("Running Game");

  uint64_t previousTicks = SDL_GetTicks64();
  uint64_t deltaTime = 0;
  bool isRunning = true;
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type==SDL_QUIT) {
        LOG_INFO("Quit event received");
        isRunning = false;
      }
    }
    // Calculate deltaTime
    uint64_t currentTicks = SDL_GetTicks64();
    deltaTime = currentTicks - previousTicks;
    previousTicks = currentTicks;

    ecs_->run_update(deltaTime);
    ecs_->run_render(deltaTime);
  }
}