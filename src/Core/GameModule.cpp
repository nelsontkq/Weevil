
#include "GameModule.h"

#include <SDL3/SDL.h>

#include "SDL3/SDL_loadso.h"

namespace fs = std::filesystem;

namespace wv {

GameModule::GameModule() {
  module_path_ = std::string(SDL_GetBasePath()) + "modules/libGameModule" +
#if defined(_WIN32)
                 ".dll";
#elif defined(__APPLE__)
                 ".dylib";
#else
                 ".so";
#endif
  if (exists(module_path_)) {
    last_write_time_ = last_write_time(module_path_);
  }
}

GameModule::~GameModule() { unload(); }

auto GameModule::load() -> bool {
  handle_ = SDL_LoadObject(module_path_.c_str());
  if (!handle_) {
    LOG_ERROR("Failed to load module '{0}': {1}", module_path_.c_str(),
              SDL_GetError());
    return false;
  }

  game_init_ = (GameInitFunc)SDL_LoadFunction(handle_, "GameInit");
  game_update_ = (GameUpdateFunc)SDL_LoadFunction(handle_, "GameUpdate");
  game_shutdown_ = SDL_LoadFunction(handle_, "GameShutdown");

  if (!game_init_ || !game_update_ || !game_shutdown_) {
    LOG_ERROR("Failed to load functions from module '{0}': {1}",
              module_path_.c_str(), SDL_GetError());
    SDL_UnloadObject(handle_);
    handle_ = nullptr;
    return false;
  }

  // Update the last modification time.
  if (fs::exists(module_path_)) {
    last_write_time_ = fs::last_write_time(module_path_);
  }
  reload_requested_ = false;
  return true;
}

void GameModule::unload() {
  if (handle_ != nullptr) {
    SDL_UnloadObject(handle_);
    handle_ = nullptr;
    game_init_ = nullptr;
    game_update_ = nullptr;
    game_shutdown_ = nullptr;
  }
}

void GameModule::init(SDL_Renderer* renderer) {
  if (game_init_) {
    game_init_(renderer);
  }
}

void GameModule::update(SDL_Renderer* renderer, const float dt) {
  if (game_update_) {
    game_update_(renderer, dt);
  }
}

void GameModule::shutdown() {
  if (game_shutdown_) {
    game_shutdown_();
  }
}

auto GameModule::needs_reload() const -> bool {
  bool fileChanged = false;
  if (exists(module_path_)) {
    fileChanged = last_write_time(module_path_) != last_write_time_;
  }
  return fileChanged || reload_requested_;
}

void GameModule::trigger_reload(SDL_Renderer* renderer) {
  LOG_INFO("Hot reloading game module...");
  shutdown();
  unload();
  if (load()) {
    init(renderer);
  } else {
    LOG_ERROR("Failed to reload game module.");
  }
}

}  // namespace wv
