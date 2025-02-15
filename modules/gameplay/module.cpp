#include <weevil/core/app_settings.h>
#include <weevil/pch.h>
namespace {
SDL_FRect square = {100, 100, 50, 50};
SDL_Color squareColor = {255, 0, 0, 255};
}  // namespace

class GameplayModule : public wv::IModule {
  void init() override { LOG_INFO("Gameplay system initialized."); }
  void update(float deltaTime) override {
    square.x += 5;
    if (square.x > 1920) {
      square.x = 0;
    }
    square.y += 6;
    if (square.y > 1080) {
      square.y = 0;
    }
  }

  void render(SDL_Renderer* renderer, float deltaTime) override {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_MAX_UINT8);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, squareColor.r, squareColor.g,
                           squareColor.b, squareColor.a);
    SDL_RenderFillRect(renderer, &square);
    SDL_RenderPresent(renderer);
  }
};
WV_MODULE(GameplayModule)