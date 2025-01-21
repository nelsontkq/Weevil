#include "WeevilEngine/Game.h"

#include "WeevilEngine/SystemManager.h"

wv::Game::Game(AppSettings& a) : settings_(a), sdlContext_(settings_) { LOG_INFO("Initializing Game"); }

wv::Game::~Game() {}

void wv::Game::run() {
  LOG_INFO("Running Game");

  uint64_t previousTicks = SDL_GetTicks();
  float deltaTime = 0;
  bool isRunning = true;
  while (isRunning) {
    SDL_Event event;
    // TODO: use EnTT signals
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_EVENT_QUIT) {
        LOG_INFO("Quit event received");
        isRunning = false;
        break;
      }
    }
    // Calculate deltaTime
    uint64_t currentTicks = SDL_GetTicks();
    deltaTime = static_cast<float>(currentTicks - previousTicks) / 1000.0f;
    previousTicks = currentTicks;
    mgr_.update(registry_, deltaTime);

    SDL_UpdateWindowSurface(sdlContext_.get_window());
  }
}
