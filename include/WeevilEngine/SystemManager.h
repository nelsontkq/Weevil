//
// Created by nelson on 1/18/25.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_
#define WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#include <string_view>
#include <vector>

#include "System.h"
#include "UUID.h"
#include "entt/entt.hpp"

namespace wv {

class SystemManager {
 public:
  explicit SystemManager(entt::registry* registry) : registry_(registry) {}
  template <SystemDerived T>
  UUID add_system();
  void init();
  void remove_system(UUID name);
  void remove_all_systems();
  void update(float deltaTime);
  void sort_systems();

 private:
  entt::registry* registry_;

  std::unordered_map<UUID, System*> systems_;
  std::vector<entt::organizer::vertex> sorted_systems_;
};

template <SystemDerived T>
UUID SystemManager::add_system() {
  UUID id;
  auto* system = new T();
  systems_.emplace(id, system);
  return id;
}

}  // namespace wv

#endif  // WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#pragma clang diagnostic pop