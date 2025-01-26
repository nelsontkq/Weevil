#pragma once

#include "UUID.h"
#include "entt/entt.hpp"
#include "AssetManager.h"
namespace wv {

struct Time {
  float deltaTime;
};
class System {
 public:
  virtual ~System() = default;
  virtual void init(wv::AssetManager &, entt::registry &) {};
  virtual auto name() -> std::string = 0;
  virtual void update(void *, entt::registry &) = 0;
  virtual void shutdown(entt::registry &) {};
};

}  // namespace wv
