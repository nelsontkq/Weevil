#pragma once
#include "pch.h"
namespace wv {
class Renderer {
 public:
  void run(entt::registry& registry, SDL_Renderer* renderer);
};
}  // namespace wv