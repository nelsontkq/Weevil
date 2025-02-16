#pragma once
#include <random>

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
};

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
  std::string text;
  int fontSize;
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
};