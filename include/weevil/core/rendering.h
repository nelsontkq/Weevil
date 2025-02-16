#pragma once
#include <SDL3/SDL_render.h>

struct RenderingContext {
  SDL_Renderer* renderer;
  int window_width;
  int window_height;
};