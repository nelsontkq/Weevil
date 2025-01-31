#include "WeevilEngine/Game.h"

#include "WeevilEngine/AssetManager.h"
#include "WeevilEngine/Components.h"
#include "WeevilEngine/InputContext.h"
#include "WeevilEngine/SystemManager.h"

wv::Game::Game(AppSettings &a)
    : settings_(a),
      sdlContext_(settings_),
      assets_(settings_, sdlContext_.get_renderer()) {
  LOG_INFO("Initializing Game");
}

void wv::Game::run() {
  LOG_INFO("Running Game");
  uint64_t previousTicks = SDL_GetTicks();
  float deltaTime = 0;
  bool isRunning = true;
  auto &player_input_context = registry_.ctx().emplace<InputContext>();

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
      player_input_context.process_event(event);
    }
    // Calculate deltaTime
    const uint64_t currentTicks = SDL_GetTicks();
    deltaTime = static_cast<float>(currentTicks - previousTicks) / 1000.0f;
    previousTicks = currentTicks;
    system_manager_.update(deltaTime);
    render();
  }
}

void wv::Game::render() {
  // TODO: tighten this up
  SDL_RenderClear(sdlContext_.get_renderer());
  auto view = registry_.view<const TransformComponent, const SpriteComponent>();
  registry_.sort<SpriteComponent>(
      [](const SpriteComponent &lhs, const SpriteComponent &rhs) {
        return lhs.z_order < rhs.z_order;
      });
  for (auto &&[entity, transform, sprite] : view.each()) {
    SDL_Texture *texture = assets_.get(sprite.idx);

    SDL_FRect dest = {transform.position.x, transform.position.y, transform.w,
                      transform.h};
    SDL_RenderTexture(sdlContext_.get_renderer(), texture, nullptr, &dest);
  }
  SDL_RenderPresent(sdlContext_.get_renderer());
}
