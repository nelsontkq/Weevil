#pragma once
#include <SDL3/SDL.h>

namespace wv {
struct SDL_Deleter {
  void operator()(SDL_Texture* p) const { SDL_DestroyTexture(p); }
  void operator()(SDL_Surface* p) const { SDL_DestroySurface(p); }
};
}  // namespace wv