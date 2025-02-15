#pragma once

class SDL_Renderer;
namespace wv {
  enum class ModulePhase {
    PREUPDATE,
    UPDATE
  };
class IModule {
 public:
  virtual ~IModule() = default;
  virtual void init(entt::registry& registry, entt::dispatcher& dispatcher) {}
  virtual void update(entt::registry& registry, entt::dispatcher& dispatcher, float deltaTime) {}
  
};

}  // namespace wv