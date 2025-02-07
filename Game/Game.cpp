#include <SDL3/SDL.h>

#include "WeevilEngine/GameModuleAPI.h"
#include "WeevilEngine/weevil.h"

// Keep some module-local state.
namespace {
SDL_FRect square = {100, 100, 50, 50};
SDL_Color squareColor = {255, 255, 0, 255};
}  // namespace

extern "C" {

void GameInit(SDL_Renderer* renderer) {
  LOG_INFO("Game module initialized.");
  // (Optionally, use the renderer to create textures, etc.)
}

void GameUpdate(SDL_Renderer* renderer, float dt) {
  // For a simple animation, move the square to the right.
  square.y += 1;
  if (square.y > 640) {  // reset when offscreen (assuming window width ~640)
    square.y = 0;
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_MAX_UINT8);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, squareColor.r, squareColor.g, squareColor.b,
                         squareColor.a);
  SDL_RenderFillRect(renderer, &square);
  SDL_RenderPresent(renderer);
}

void GameShutdown() {}
}
