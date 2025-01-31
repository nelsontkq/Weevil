//
// Created by nelson on 1/31/25.
//
#include "WeevilEngine/Renderer.h"

#include <entt/entity/registry.hpp>

auto wv::Renderer::render(SDL_Renderer *renderer, entt::registry &registry)
    -> void {
  // TODO: tighten this up
  SDL_RenderClear(renderer);
  // auto view = registry.view<const TransformComponent, const SpriteComponent>();
  // registry.sort<SpriteComponent>(
  //     [](const SpriteComponent &lhs, const SpriteComponent &rhs) {
  //       return lhs.z_order < rhs.z_order;
  //     });
  // for (auto &&[entity, transform, sprite] : view.each()) {
  //   SDL_Texture *texture = assets_.get(sprite.idx);
  //
  //   SDL_FRect dest = {transform.position.x, transform.position.y, transform.w,
  //                     transform.h};
  //   SDL_RenderTexture(renderer, texture, nullptr, &dest);
  // }
  SDL_RenderPresent(renderer);
}
