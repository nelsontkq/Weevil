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
  explicit Game(AppSettings& a);
  void run();

 private:
  void render();
  entt::registry registry_;
  SystemManager system_manager_{&registry_};
  AppSettings& settings_;
  SDLContext sdlContext_;
  AssetManager assets_;
};
}  // namespace wv
