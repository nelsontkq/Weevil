#include <weevil/weevil_api.h>
struct FrameCounter {};
constexpr auto DEFAULT_FONT = "default_font";
class DebugModule : public wv::IModule {
 public:
  void init(entt::dispatcher& dispatcher) override {
    LOG_INFO("DebugModule::init");
    // add FPS counter
    auto entity = registry.create();
    registry.emplace<FrameCounter>(entity);
    registry.emplace<wv::Transform>(entity, wv::Vector2{1.0f, 1.0f}, wv::Size{10.0f, 10.0f});
    uint8_t max_val = std::numeric_limits<uint8_t>::max();
    registry.emplace<wv::Color>(entity, max_val, max_val, max_val, max_val);
    dispatcher.enqueue<wv::LoadFont>(DEFAULT_FONT, "assets/fonts/Anybody-Medium.ttf", 16.0f);
    registry.emplace<wv::Text>(entity, DEFAULT_FONT, "FPS: 0");
  }

  void update(entt::dispatcher& dispatcher, float dt) override {
    auto view = registry.view<const FrameCounter, wv::Text>();
    dt = 0;
    for (auto [entity, text] : view.each()) {
      text.value = std::format("FPS: {:.2f}", dt / 1.0f);
    }
  }

  void shutdown(entt::dispatcher& dispatcher) override { LOG_INFO("DebugModule::shutdown"); }
};
WV_MODULE(DebugModule)