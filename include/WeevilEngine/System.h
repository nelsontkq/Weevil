#pragma once

#include "AssetManager.h"
#include "UUID.h"
#include "entt/entt.hpp"
namespace wv {

struct Time {
  float deltaTime;
};
class System {
 public:
  virtual ~System() = default;
  System(System &&) = default;
  auto operator=(System &&) -> System & = default;
  System(const System &) = delete;
  auto operator=(const System &) -> System & = delete;

  virtual void init(AssetManager &, entt::registry &) {}
  virtual auto name() -> std::string = 0;
  virtual void update(void *, entt::registry &) = 0;
  virtual void shutdown(entt::registry &) {}
};

}  // namespace wv
