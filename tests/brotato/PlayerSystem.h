#pragma once

#include "WeevilEngine/System.h"
#include "WeevilEngine/Components.h"
#include "WeevilEngine/InputContext.h"

class PlayerSystem : public wv::System {
 public:
  auto name() -> std::string override { return "PlayerSystem"; }
  void init(wv::AssetManager &, entt::registry &) override;
  void update(void *, entt::registry &) override;
  void shutdown(entt::registry &) override;

 private:
  entt::entity player_entity_{entt::null};
  uint64_t player_tex_;
};
