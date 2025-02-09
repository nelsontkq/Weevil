#pragma once

class SDL_Renderer;
namespace wv {
// This class is a module that can be loaded by the application.
class IModule {
 public:
  virtual ~IModule() = default;
  virtual void init() {}
  virtual void preupdate(float deltaTime) {}
  virtual void update(float deltaTime) {}
  virtual void render(SDL_Renderer* renderer, float deltaTime) {}
  virtual void shutdown() {}
};

}  // namespace wv