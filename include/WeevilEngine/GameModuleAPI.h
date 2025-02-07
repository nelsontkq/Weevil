#pragma once
#include <SDL3/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

  // Called once when the module is loaded.
  // The engine passes in its SDL_Renderer* so the module can draw.
  void GameInit(SDL_Renderer* renderer);

  // Called every frame; dt is the frame time in seconds.
  void GameUpdate(SDL_Renderer* renderer, float dt);

  // Called when the module is about to be unloaded.
  void GameShutdown();

#ifdef __cplusplus
}
#endif
