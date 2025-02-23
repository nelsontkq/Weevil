#pragma once
#include <SDL3/SDL_render.h>
namespace wv {
struct Window {
  int width;
  int height;
  SDL_Renderer* renderer;
};
}  // namespace wv