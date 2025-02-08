#include "WeevilEngine/weevil.h"

namespace {
SDL_FRect square = {100, 100, 50, 50};
SDL_Color squareColor = {255, 0, 0, 255};
}

extern "C" {

void GameInit() { LOG_INFO("Game module initialized. :("); }

void GameUpdate(SDL_Renderer* renderer, float deltaTime) {
  square.x += 5;
  if (square.x > 1920) {
    square.x = 0;
  }
  square.y += 1;
  if (square.y > 1080) {
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
