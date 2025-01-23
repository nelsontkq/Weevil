#include "WeevilEngine/Game.h"

#include "WeevilEngine/AssetManager.h"
#include "WeevilEngine/Components.h"
#include "WeevilEngine/SystemManager.h"

wv::Game::Game(AppSettings& a)
    : settings_(a),
      sdlContext_(settings_),
      assets_(settings_, sdlContext_.get_renderer()),
      system_manager_(&registry_) {
  LOG_INFO("Initializing Game");
}

wv::Game::~Game() {}

void wv::Game::run() {
  LOG_INFO("Running Game");
  uint64_t previousTicks = SDL_GetTicks();
  float deltaTime = 0;
  bool isRunning = true;
  system_manager_.init(assets_);
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
    system_manager_.update(deltaTime);
    SDL_RenderClear(sdlContext_.get_renderer());
    auto view = registry_.view<const TransformComponent, const SpriteComponent>();
    for (auto&& [entity, transform, sprite] : view.each()) {
      SDL_Texture* texture = assets_.get(sprite.idx);

      SDL_FRect dest = {transform.position.x, transform.position.y, transform.w, transform.h};
      SDL_RenderTexture(sdlContext_.get_renderer(), texture, nullptr, &dest);
    }
    SDL_RenderPresent(sdlContext_.get_renderer());
  }
}
