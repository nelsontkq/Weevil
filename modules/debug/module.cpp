#include <weevil/weevil_api.h>
struct FrameCounter {};
class DebugModule : public wv::IModule {
 public:
  void init(entt::registry& registry, entt::dispatcher& dispatcher) {
    LOG_INFO("DebugModule::init");
    // add FPS counter
    auto entity = registry.create();
    registry.emplace<FrameCounter>(entity);
    registry.emplace<wv::Transform>(entity, wv::Vector2{1, 1}, wv::Size{10, 10});
    registry.emplace<wv::Color>(entity, 255, 255, 255, 255);
    registry.emplace<wv::Text>(entity, "assets/fonts/Anybody-Medium.ttf", 16.0, "FPS: 0");
  }

  void update(entt::registry& registry, entt::dispatcher& dispatcher, float dt) {
    auto view = registry.view<const FrameCounter, wv::Text>();
    for (auto [entity, text] : view.each()) {
      text.value = std::format("FPS: {:.2f}", 1.0f / (dt + std::numeric_limits<float>::epsilon()));
    }
  }

  void shutdown(entt::registry& registry, entt::dispatcher& dispatcher) { LOG_INFO("DebugModule::shutdown"); }
};
WV_MODULE(DebugModule)