#ifndef WV_GAME_HPP
#define WV_GAME_HPP

#include "wvpch.h"
#include "SDLContext.h"
#include "SystemManager.h"

namespace wv {

class Game {
 public:
  Game();

  ~Game();

  void run();

 private:
  static bool logInitialized_;

  static bool initializeLog();

  AppSettings settings_;
  SDLContext sdlContext_;
  entt::registry registry_;
  // tasks that start and end;
  entt::basic_scheduler<uint64_t> scheduler_;
  SystemManager mgr_;
};
}

#endif // WV_GAME_HPP
