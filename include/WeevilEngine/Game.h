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
  AppSettings& settings_;
  SDLContext sdlContext_;
  AssetManager assets_;
};
}  // namespace wv
