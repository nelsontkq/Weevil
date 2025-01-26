#pragma once

#include <concepts>

#include "AssetManager.h"
#include "NoCopy.h"
#include "SDLContext.h"
#include "System.h"
#include "SystemManager.h"
#include "Scene.h"
#include "wvpch.h"

namespace wv {

class Game : NoCopy {
 public:
  Game(AppSettings& a);
  void run();
  template <Derived<Scene> SceneType, typename... Args>
  void push_scene(Args&&... args) {
    scene_manager_.push<SceneType>(std::forward<Args>(args)...);
  }

  template <typename SceneType, typename... Args>
  void change_scene(Args&&... args) {
    scene_manager_.change<SceneType>(std::forward<Args>(args)...);
  }

  template <typename SceneType, typename... Args>
  void pop_scene(Args&&... args) {
    scene_manager_.pop<SceneType>(std::forward<Args>(args)...);
  }

 private:
  void render();
  AppSettings& settings_;
  SDLContext sdlContext_;
  SceneManager scene_manager_;
  AssetManager assets_;
};
}  // namespace wv
