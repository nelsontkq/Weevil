//
// Created by nelson on 1/18/25.
//

#ifndef WEEVIL_INCLUDE_WEEVILENGINE_SYSTEM_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_SYSTEM_H_
#include "entt/entt.hpp"
#include "UUID.h"
namespace wv {
enum Stage {
  PreUpdate,
  Update,
  Render
};
struct SystemContext {
  float deltaTime;
};
class System {
 public:
  virtual void init(entt::registry &registry) = 0;
  virtual std::vector<UUID> after() = 0;
  virtual Stage stage() = 0;
  virtual void update(wv::SystemContext *data, entt::registry &registry) = 0;
};
}
#endif //WEEVIL_INCLUDE_WEEVILENGINE_SYSTEM_H_
