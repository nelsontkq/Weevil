#pragma once
#include "WeevilEngine/Scene.h"

class MainGameScene : wv::Scene {
  ~MainGameScene();

  void on_enter(entt::registry& registry);
  void on_exit();
};
