#pragma once
#include <cstdint>
namespace wv {
// Vector2 with the origin at the top-left corner
struct Vector2 {
  float x, y;
};

// Screen location
struct TransformComponent {
  Vector2 position;
  float w, h;
};

struct SpriteComponent {
  uint64_t idx;
  int z_order = 0;
};
}  // namespace wv
