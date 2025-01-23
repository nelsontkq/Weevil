//
// Created by nelson on 1/18/25.
//

#ifndef WEEVIL_INCLUDE_WEEVILENGINE_SYSTEM_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_SYSTEM_H_
#include "UUID.h"
#include "entt/entt.hpp"
namespace wv {

struct SystemContext {
  float deltaTime;
};
class System {
 public:
  virtual ~System() = default;
  virtual void init(entt::registry &) {};
  virtual std::string name() = 0;
  virtual void update(void *, entt::registry &) = 0;
  virtual void shutdown(entt::registry &) {};
};
template <typename Derived>
concept SystemDerived = std::is_base_of_v<System, Derived>;

}  // namespace wv
#endif  // WEEVIL_INCLUDE_WEEVILENGINE_SYSTEM_H_
