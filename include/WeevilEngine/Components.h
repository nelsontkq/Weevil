//
// Created by nelson on 1/18/25.
//

#ifndef WEEVIL_INCLUDE_WEEVILENGINE_COMPONENTS_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_COMPONENTS_H_
#include "wvpch.h"

// Vector2 with the origin at the top-left corner
struct Vector2 {
  float x, y;
};

struct TransformComponent {
  Vector2 position;
  Vector2 rotation;
  Vector2 scale;
};

struct SpriteComponent {
};
#endif //WEEVIL_INCLUDE_WEEVILENGINE_COMPONENTS_H_
