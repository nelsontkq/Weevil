#include "renderer.h"

#include <weevil/core/components.h>
#include <weevil/core/window.h>

#include "pch.h"

void wv::Renderer::run(entt::registry& registry, SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  for (auto [entity, transform, color] : registry.view<const wv::Transform, const wv::Color>().each()) {
    SDL_FRect rect = {transform.position.x, transform.position.y, transform.size.width, transform.size.height};

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
  }
  SDL_RenderPresent(renderer);
}