#include <weevil/weevil_api.h>

struct Rectangle {};

class GameplayModule : public wv::IModule {
 public:
  void init(entt::registry& registry, entt::dispatcher& dispatcher) {
    LOG_INFO("GameplayModule::init");
    auto rend = registry.ctx().get<RenderingContext>();
    width_ = rend.window_width;
    height_ = rend.window_height;

    auto rng = registry.ctx().get<Rngen>();
    auto entity = registry.create();
    registry.emplace<Rectangle>(entity);
    registry.emplace<Transform>(entity, rng.random<Transform>(width_, height_, 10, 50));
    registry.emplace<Color>(entity, rng.random<Color>());
  }
  void update(entt::registry& registry, entt::dispatcher& dispatcher, float dt) {
    // spawn a new entity every 2 seconds
    static float timer = 0;
    timer += dt;
    if (timer > 2) {
      timer = 0;
      auto entity = registry.create();
      registry.emplace<Rectangle>(entity);
      registry.emplace<Transform>(entity, registry.ctx().get<Rngen>().random<Transform>(width_, height_, 10, 50));
      registry.emplace<Color>(entity, registry.ctx().get<Rngen>().random<Color>());
    }
    auto view = registry.view<const Rectangle, Transform>();
    auto rng = registry.ctx().get<Rngen>();
    for (auto entity : view) {
      auto& transform = view.get<Transform>(entity);
      transform.position.x += rng.random<float>(-2.0, 2.0);
      if (transform.position.x > width_) {
        transform.position.x = 0;
      } else if (transform.position.x < 0) {
        transform.position.x = width_;
      }
      transform.position.y += rng.random<float>(-2.0, 2.0);
      if (transform.position.y > height_) {
        transform.position.y = 0;
      } else if (transform.position.y < 0) {
        transform.position.y = height_;
      }
    }
  }

  void shutdown(entt::registry& registry, entt::dispatcher& dispatcher) { LOG_INFO("GameplayModule::shutdown"); }

 private:
  int width_;
  int height_;
};

WV_MODULE(GameplayModule)