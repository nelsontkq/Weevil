//
// Created by nelson on 1/18/25.
//

#ifndef WEEVIL_INCLUDE_WEEVILENGINE_COMPONENTS_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_COMPONENTS_H_
#include "wvpch.h"
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
};
}  // namespace wv
#endif  // WEEVIL_INCLUDE_WEEVILENGINE_COMPONENTS_H_
