#include <weevil/weevil_api.h>

struct Rectangle {};

class GameplayModule : public wv::IModule {
 public:
  void init(entt::registry& registry, entt::dispatcher& dispatcher) override {
    LOG_INFO("GameplayModule::init");
    const auto rend = registry.ctx().get<wv::Window>();
    width_ = rend.width;
    height_ = rend.height;

    add_random_rectangle(registry, 20000);
  }
  void update(entt::registry& registry, entt::dispatcher& dispatcher, float dt) override {
    // spawn a new entity every 2 seconds
    static float timer = 0;
    timer += dt;
    if (timer > 2) {
      timer = 0;
      add_random_rectangle(registry, 1);
    }
    auto view = registry.view<const Rectangle, wv::Transform>();
    auto& rng = registry.ctx().get<Rngen>();
    for (auto entity : view) {
      auto& transform = view.get<wv::Transform>(entity);
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

  void shutdown(entt::registry& registry, entt::dispatcher& dispatcher) override {
    LOG_INFO("GameplayModule::shutdown");
  }

 private:
  void add_random_rectangle(entt::registry& registry, size_t count = 1) {
    for (size_t i = 0; i < count; i++) {
      auto entity = registry.create();
      registry.emplace<Rectangle>(entity);
      registry.emplace<wv::Transform>(entity,
                                      registry.ctx().get<Rngen>().random<wv::Transform>(width_, height_, 10, 50));
      registry.emplace<wv::Color>(entity, registry.ctx().get<Rngen>().random<wv::Color>());
    }
  }
  int width_;
  int height_;
};

WV_MODULE(GameplayModule)