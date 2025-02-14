#pragma once

class SDL_Renderer;
namespace wv {
// This class is a module that can be loaded by the application.
class IModule {
 public:
  virtual ~IModule() = default;
  virtual void init() {}
  virtual void preupdate(float) {}
  virtual void update(float) {}
  virtual void render(SDL_Renderer*, float) {}
  virtual void shutdown() {}
  virtual void process_event(SDL_Event&) {}
};

}  // namespace wv