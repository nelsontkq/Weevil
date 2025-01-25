#pragma once

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
  AssetManager assets_;
  SystemManager system_manager_;
};
}  // namespace wv
