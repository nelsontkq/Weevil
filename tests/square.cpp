#include "WeevilEngine/AssetManager.h"
#include "WeevilEngine/Components.h"
#include "WeevilEngine/Game.h"

class SquareSystem : public wv::System {
 public:
  auto name() -> std::string override { return "SquareSystem"; }
  void init(wv::AssetManager &assets, entt::registry &registry) override {
    const auto square = registry.create();
    registry.emplace<wv::TransformComponent>(square, wv::Vector2{.x = 0, .y = 0}, 100.0f, 100.0f);
    idx_ = assets.load("textures/square.png");
    registry.emplace<wv::SpriteComponent>(square, idx_);
  }
  // Returns a list of UUIDs of the components that this system depends on
  void update(void *d, entt::registry &registry) override {
    auto view = registry.view<const wv::SpriteComponent, wv::TransformComponent>();
    auto data = registry.ctx().get<wv::SystemContext>();

    for (auto [ent, sp, t] : view.each()) {
      t.position.x += 1 * data.deltaTime;
      t.position.y += 1 * data.deltaTime;
    }
  }
  void shutdown(entt::registry &registry) override {
    auto assets = registry.ctx().get<wv::AssetManager>();
    assets.unload(idx_);
  }

 private:
  uint64_t idx_;
};