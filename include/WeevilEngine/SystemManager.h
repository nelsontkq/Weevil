#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include "AssetManager.h"
#include "NoCopy.h"
#include "System.h"
#include "weevil.h"

namespace wv {

class SystemManager : NoCopy {
 public:
  explicit SystemManager(entt::registry& registry);
  template <Derived<System> T>
  auto add_system() -> void {
    T* sys = new T();
    systems_.insert(sys);
  }
  void init(AssetManager& assets);
  void update(float deltaTime);
  void remove_all_systems();
  void sort_systems();
  void shutdown();

 private:
  entt::registry* registry_;
  entt::organizer organizer_;

  entt::dense_set<System*> systems_;
  std::vector<entt::organizer::vertex> sorted_systems_;
};

}  // namespace wv

#pragma clang diagnostic pop