#include "player.h"

#include <entt/entt.hpp>

PlayerSystem::~PlayerSystem() {}

void PlayerSystem::init(wv::AssetManager &assets, entt::registry &registry) {
  auto player = registry.create();
  playerTex_ = assets.load("textures/square.png");
  registry.emplace<wv::TransformComponent>(player, wv::Vector2{400.0f, 300.0f}, 64.0f, 64.0f);
  registry.emplace<wv::SpriteComponent>(player, playerTex_);
  registry.emplace<HealthComponent>(player, HealthComponent{100.0f, 100.0f});
  registry.emplace<PlayerComponent>(player, PlayerComponent{200.0f, 0.0f, 1});

  playerEntity_ = player;
}

void PlayerSystem::update(void *d, entt::registry &registry) {
  if (playerEntity_ == entt::null) {
    return;
  }
  auto &transform = registry.get<wv::TransformComponent>(playerEntity_);
  auto &playerComp = registry.get<PlayerComponent>(playerEntity_);

  // movement here
}

void PlayerSystem::shutdown(entt::registry &registry) {
  // Optionally unload the texture
  auto assets = registry.ctx().get<wv::AssetManager>();
  assets.unload(playerTex_);
}
