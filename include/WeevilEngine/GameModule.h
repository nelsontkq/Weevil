#pragma once
#include <SDL3/SDL.h>

#include "WeevilEngine/weevil.h"

namespace wv {

class GameModule {
 public:
  explicit GameModule();
  ~GameModule();

  // Load the dynamic library and resolve the required symbols.
  bool load();

  // Unload the library.
  void unload();

  // Call the module’s functions.
  void init(SDL_Renderer* renderer);
  void update(SDL_Renderer* renderer, float dt);
  void shutdown();

  // Checks if the file on disk has been modified (or a reload was triggered).
  bool needs_reload() const;

  // Optionally trigger reload from inside the game code.
  void trigger_reload();

 private:
  std::filesystem::path module_path_;
  SDL_SharedObject* handle_ = nullptr;

  // Function pointers to the module functions.
  using GameInitFunc = void (*)(SDL_Renderer*);
  using GameUpdateFunc = void (*)(SDL_Renderer*, float);
  using GameShutdownFunc = void (*)();

  GameInitFunc game_init_ = nullptr;
  GameUpdateFunc game_update_ = nullptr;
  GameShutdownFunc game_shutdown_ = nullptr;

  // Last modification time of the module file.
  std::filesystem::file_time_type last_write_time_;
  bool reload_requested_ = false;
};

}  // namespace wv
