#ifndef WV_GAME_HPP
#define WV_GAME_HPP
#include <concepts>

#include "AssetManager.h"
#include "NoCopy.h"
#include "SDLContext.h"
#include "System.h"
#include "SystemManager.h"
#include "wvpch.h"
namespace wv {

class Game : NoCopy {
 public:
  Game(AppSettings& a);
  ~Game();
  SystemManager& system_manager() { return system_manager_; }

  void run();

 private:
  void render();
  AppSettings& settings_;
  SDLContext sdlContext_;
  entt::registry registry_;
  // tasks that start and end;
  entt::basic_scheduler<uint64_t> scheduler_;
  AssetManager assets_;
  SystemManager system_manager_;
};
}  // namespace wv

#endif  // WV_GAME_HPP
