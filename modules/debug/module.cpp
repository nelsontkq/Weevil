#include <SDL3/SDL_timer.h>
#include <weevil/weevil_api.h>
struct FrameCounter {};
class DebugModule : public wv::IModule {
 public:
  void init(entt::dispatcher& dispatcher) override {
    LOG_INFO("DebugModule::init");
    // add FPS counter
    auto entity = registry.create();
    registry.emplace<FrameCounter>(entity);
    registry.emplace<wv::Transform>(entity, wv::Vector2{1.0f, 1.0f}, wv::Size{280.0f, 120.0f});
    uint8_t max_val = std::numeric_limits<uint8_t>::max();
    dispatcher.enqueue<wv::LoadFont>("default_font", "assets/fonts/Anybody-Medium.ttf", 72.0f);
    registry.emplace<wv::Text>(entity, "default_font", "FPS: 0");
  }

  void update(entt::dispatcher& dispatcher, float dt) override {
    auto view = registry.view<const FrameCounter, wv::Text>();
    static float fps = 0.0f;
    static float frame_count = 0.0f;
    static float elapsed_time = 0.0f;
    elapsed_time += dt;
    frame_count++;
    if (elapsed_time >= 1.0f) {
      fps = frame_count / elapsed_time;
      frame_count = 0.0f;
      elapsed_time = 0.0f;
    }
    for (auto [entity, text] : view.each()) {
      text.value = std::format("FPS: {:.2f}", fps);
    }
  }

  void shutdown(entt::dispatcher& dispatcher) override { 
    dispatcher.enqueue<wv::UnloadFont>("default_font"); 
  }
};
WV_MODULE(DebugModule)