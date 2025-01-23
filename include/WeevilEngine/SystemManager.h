//
// Created by nelson on 1/18/25.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_
#define WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#include "System.h"
#include "NoCopy.h"
#include "wvpch.h"

namespace wv {

class SystemManager : NoCopy {
 public:

  explicit SystemManager(entt::registry* registry) : registry_(registry) {}
  template <SystemDerived T>
  UUID add_system() {
    UUID id;
    auto* sys = new T();
    systems_[id] = sys;

    auto y = systems_.size();
    return id;
  }
  void init();
  void remove_system(UUID name);
  void remove_all_systems();
  void update(float deltaTime);
  void sort_systems();

 private:
  entt::registry* registry_;
  SystemContext* ctx_;
  entt::organizer organizer_;

  std::unordered_map<UUID, System*> systems_;
  std::vector<entt::organizer::vertex> sorted_systems_;
};

}  // namespace wv

#endif  // WEEVIL_SRC_SYSTEM_SYSTEMMANAGER_H_

#pragma clang diagnostic pop