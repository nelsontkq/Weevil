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

struct Transform {
  Vector2 position;
  Size size;

  static Transform random(std::mt19937 &engine, float windowWidth, float windowHeight, float minSize, float maxSize) {
    auto pos = Vector2::random(engine, windowWidth, windowHeight);

    std::uniform_real_distribution<float> distSize(minSize, maxSize);
    auto size = Size{distSize(engine), distSize(engine)};
    return {pos, size};
  }

  operator SDL_FRect() const { return {.x = position.x, .y = position.y, .w = size.width, .h = size.height}; };
};

struct Clip {
  Vector2 position;
  Size size;
  operator SDL_FRect() const { return {.x = position.x, .y = position.y, .w = size.width, .h = size.height}; };
};

}  // namespace wv