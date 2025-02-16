#include <weevil/weevil_api.h>

class DebugModule : public wv::IModule {
 public:
  void init(entt::registry& registry, entt::dispatcher& dispatcher) {
    LOG_INFO("DebugModule::init");
    // add FPS counter
    auto entity = registry.create();
    registry.emplace<Transform>(entity, Vector2{1, 1}, Size{10, 10});
    registry.emplace<Color>(entity, Color{255, 255, 255, 255});
    registry.emplace<Text>(entity, "FPS: 0", 0);
  }

  void update(entt::registry& registry, entt::dispatcher& dispatcher, float dt) {
    // update FPS counter
    auto view = registry.view<Text>();
    for (auto entity : view) {
      auto& text = view.get<Text>(entity);
      text.text = "FPS: " + std::to_string((int)(1.0f / dt));
    }
  }

  void shutdown(entt::registry& registry, entt::dispatcher& dispatcher) { LOG_INFO("DebugModule::shutdown"); }
};
WV_MODULE(DebugModule)