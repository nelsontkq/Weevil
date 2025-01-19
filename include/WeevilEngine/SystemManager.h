//
// Created by nelson on 1/18/25.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_
#define WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#include <vector>
#include <string_view>
#include "entt/entt.hpp"
#include "System.h"
#include "UUID.h"

namespace wv {


class SystemManager {
 public:
  UUID add_system(System&& sys);
  void remove_system(UUID name);
  void remove_all_systems();
  void update(entt::registry &registry, float deltaTime);
  void sort_systems();

 private:
  std::vector<System> systems_;
  std::vector<entt::organizer::vertex> sorted_systems_;
};

} // wv

#endif //WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#pragma clang diagnostic pop