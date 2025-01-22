#include "WeevilEngine/Game.h"
#include "WeevilEngine/Components.h"
#include "WeevilEngine/AssetManager.h"

class SquareSystem : public wv::System {
 public:
  void init(entt::registry &registry) override {
    const auto square = registry.create();
    auto assets = registry.ctx().get<wv::AssetManager>();
    auto pos = wv::Vector2{.x=0, .y=0};
    auto rotation = wv::Vector2{.x=0, .y=0};
    auto scale = wv::Vector2{.x=0, .y=0};
    registry.emplace<wv::TransformComponent>(square, pos, rotation, scale);
    idx_ = assets.load("textures/square.png");
    registry.emplace<wv::SpriteComponent>(square, idx_);
  }
  // Returns a list of UUIDs of the components that this system depends on
  void update(wv::SystemContext *data, entt::registry &registry) override {
    auto view = registry.view<const wv::SpriteComponent, wv::TransformComponent>();
    for (auto [ent, sp, t] : view.each()) {
      t.rotation.y += 5*data->deltaTime;
    }
  }
  void shutdown(entt::registry &registry) override {
    auto assets = registry.ctx().get<wv::AssetManager>();
    assets.unload(idx_);
  }
 private:
  uint64_t idx_;
};

auto main() -> int {
  wv::Log::Init();
  wv::AppSettings settings("/home/nelson/Development/Weevil/config/weevil.toml");
  wv::Game game(settings);
  game.system_manager().add_system<SquareSystem>();
  game.run();
  return 0;
}
