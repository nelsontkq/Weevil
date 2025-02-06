#pragma once
#include <SDL3/SDL.h>

#include <entt/entity/registry.hpp>
namespace wv {
class Renderer {
  public:
  auto render(SDL_Renderer*, entt::registry& registry) -> void;
};
}