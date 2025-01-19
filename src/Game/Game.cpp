#include "WeevilEngine/Game.h"

#include "WeevilEngine/SystemManager.h"

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
}

wv::Game::~Game() {
}

void wv::Game::run() {
  LOG_INFO("Running Game");

  uint64_t previousTicks = SDL_GetTicks64();
  float deltaTime = 0;
  bool isRunning = true;
  while (isRunning) {
    SDL_Event event;
    // TODO: use EnTT signals
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        LOG_INFO("Quit event received");
        isRunning = false;
      }
    }
    // Calculate deltaTime
    uint64_t currentTicks = SDL_GetTicks64();
    deltaTime = static_cast<float>(currentTicks - previousTicks) / 1000.0f;
    previousTicks = currentTicks;
    mgr_.update(registry_, deltaTime);
  }
}
