#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "AssetManager.h"
#include "NoCopy.h"
#include "System.h"
#include "wvpch.h"

namespace wv {

class SystemManager : NoCopy {
 public:
  explicit SystemManager(entt::registry* registry) : registry_(registry) {}

  template <Derived<System> T>
  UUID add_system() {
    UUID id;
    auto* sys = new T();
    systems_[id] = sys;
    return id;
  }
  void init(AssetManager& assets);
  void remove_system(UUID name);
  void remove_all_systems();
  void update(float deltaTime);
  void sort_systems();

 private:
  entt::registry* registry_;
  entt::organizer organizer_;

  std::unordered_map<UUID, System*> systems_;
  std::vector<entt::organizer::vertex> sorted_systems_;
};

}  // namespace wv

#pragma clang diagnostic pop