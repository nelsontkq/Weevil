#pragma once
#include <core/sdl_deleter.h>
#include <weevil/core/components.h>

#include "pch.h"
namespace wv {
class Texture {
 public:
  virtual void render(SDL_Renderer* renderer, const wv::Transform& transform) = 0;
};
}  // namespace wv