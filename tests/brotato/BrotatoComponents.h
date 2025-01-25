#pragma once

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