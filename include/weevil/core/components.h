#pragma once
#include <SDL3/SDL.h>

#include <random>

namespace wv {
struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;

  static Color random(std::mt19937 &engine) {
    std::uniform_int_distribution<int> dist(0, std::numeric_limits<uint8_t>::max());
    return {static_cast<uint8_t>(dist(engine)), static_cast<uint8_t>(dist(engine)), static_cast<uint8_t>(dist(engine)),
            std::numeric_limits<uint8_t>::max()};
  }
  operator SDL_Color() const { return SDL_Color{r, g, b, a}; }
};

struct Rectangle {};

struct Vector2 {
  float x;
  float y;

  static Vector2 random(std::mt19937 &engine, float maxX, float maxY) {
    std::uniform_real_distribution<float> distX(0.0f, maxX);
    std::uniform_real_distribution<float> distY(0.0f, maxY);
    return {distX(engine), distY(engine)};
  }
};
struct Size {
  float width;
  float height;
};
struct Text {
  std::string_view asset_name;
  std::string value;
};
struct Sprite {
  std::string asset_name;
  // Which index of the spritesheet, if used.
  int frame{-1};
};
struct Animation {
  int start;
  int end;
  float frame_time;
  float current{0.0f};
};
struct Velocity {
  float x;
  float y;
  float target_x;
  float target_y;
};
struct Transform {
  Vector2 position;
  Size size;
  float rotation{0.0f};

  static Transform random(std::mt19937 &engine, float windowWidth, float windowHeight, float minSize, float maxSize) {
    wv::Vector2 pos{Vector2::random(engine, windowWidth, windowHeight)};

    std::uniform_real_distribution<float> dist_size(minSize, maxSize);
    wv::Size size{dist_size(engine), dist_size(engine)};
    return {pos, size};
  }

  operator SDL_FRect() const { return {.x = position.x, .y = position.y, .w = size.width, .h = size.height}; };
};

struct Clip {
  SDL_FRect rect;
};

}  // namespace wv