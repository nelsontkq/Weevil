#pragma once
#include <entt/entt.hpp>
namespace wv {

class IModule {
 public:
  virtual ~IModule() = default;
  virtual void init(entt::registry& registry, entt::dispatcher& dispatcher) {}
  virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float dt) {}
  virtual void shutdown(entt::registry& registry, entt::dispatcher& dispatcher) {}
};

}  // namespace wv
