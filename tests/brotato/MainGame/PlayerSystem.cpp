#include "PlayerSystem.h"

#include <entt/entt.hpp>

#include "../BrotatoComponents.h"

void PlayerSystem::init(wv::AssetManager &assets, entt::registry &registry) {
  auto player = registry.create();
  player_tex_ = assets.load("textures/player/potato.png");
  registry.emplace<wv::TransformComponent>(player, wv::Vector2{400.0f, 300.0f}, 64.0f, 64.0f);
  registry.emplace<wv::SpriteComponent>(player, player_tex_);
  registry.emplace<HealthComponent>(player, HealthComponent{.current = 100.0f, .max = 100.0f});
  registry.emplace<PlayerComponent>(player, PlayerComponent{.speed = 15.0f, .experience = 0.0f, .level = 1});

  player_entity_ = player;
}

void PlayerSystem::update(void *d, entt::registry &registry) {
  auto &transform = registry.get<wv::TransformComponent>(player_entity_);
  auto &playerComp = registry.get<PlayerComponent>(player_entity_);
}

void PlayerSystem::shutdown(entt::registry &registry) {
  // Optionally unload the texture
  auto assets = registry.ctx().get<wv::AssetManager>();
  assets.unload(player_tex_);
}
