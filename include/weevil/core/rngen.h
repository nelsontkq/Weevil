#pragma once

template <typename T, typename Engine, typename... Args>
concept Randomizable = requires(Engine& engine, Args&&... args) {
  { T::random(engine, std::forward<Args>(args)...) } -> std::convertible_to<T>;
};
template <typename T>
concept Integral = std::is_integral_v<T>;
template <typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

class Rngen {
 public:
  Rngen() : engine(std::random_device{}()) {}
  template <typename T, typename... Args>
    requires Randomizable<T, std::mt19937, Args...>
  T random(Args&&... args) {
    return T::random(engine, std::forward<Args>(args)...);
  }

  Integral auto random(Integral auto min, Integral auto max) {
    std::uniform_int_distribution dist(min, max);
    return dist(engine);
  }

  template <FloatingPoint T>
  T random(T min, T max) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(engine);
  }

 private:
  std::mt19937 engine;
};
