#pragma once
#include "WeevilEngine/InputContext.h"
#include "WeevilEngine/System.h"

class InputSystem : public wv::System {
 public:
  auto name() -> std::string override { return "PlayerSystem"; }
  void init(wv::AssetManager &, entt::registry &) override;
  void update(void *, entt::registry &) override;
  void shutdown(entt::registry &) override;

 private:
  wv::InputContext *input;
};
