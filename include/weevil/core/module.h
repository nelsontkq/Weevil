#pragma once
#include <entt/entt.hpp>
namespace wv {

class Module {
 public:
  virtual ~Module() = default;
  virtual void init(entt::dispatcher& dispatcher) {}
  virtual void update(entt::dispatcher& dispatcher, float dt) {}
  virtual void shutdown(entt::dispatcher& dispatcher) {}
  entt::registry* registry;
};

}  // namespace wv
