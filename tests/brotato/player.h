#pragma once

#include "WeevilEngine/System.h"
#include "WeevilEngine/Components.h"

struct HealthComponent {
  float current;
  float max;
};

struct EnemyComponent {
  float speed;
};

struct ProjectileComponent {
  float speed;
  float damage;
  wv::Vector2 direction;
  float lifetime;
};

struct PlayerComponent {
  float speed;
  float experience;
  int level;
};
class PlayerSystem : public wv::System {
 public:
  ~PlayerSystem() override;
  auto name() -> std::string override { return "PlayerSystem"; }
  virtual void init(wv::AssetManager &, entt::registry &) override;
  virtual void update(void *, entt::registry &) override;
  virtual void shutdown(entt::registry &) override;

 private:
  entt::entity playerEntity_{entt::null};
  uint64_t playerTex_;
};
