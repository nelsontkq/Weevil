#include <weevil/weevil_api.h>

class GameplayModule : public wv::Module {
 public:
  void init(entt::dispatcher& dispatcher) override {
    LOG_INFO("GameplayModule::init");
    wv::Size ship_size{66.0f, 113.0f};
    std::vector<wv::Transform> sprites{
        {.position{408.0f, 0.0f}, .size{ship_size}},   {.position{340.0f, 345.0f}, .size{ship_size}},
        {.position{340.0f, 230.0f}, .size{ship_size}}, {.position{340.0f, 115.0f}, .size{ship_size}},
        {.position{340.0f, 0.0f}, .size{ship_size}},
    };
    dispatcher.enqueue<wv::LoadSpriteSheet>("ships", "assets/textures/ships.png", sprites);

    const auto rend = registry->ctx().get<wv::Window>();
    width_ = rend.width;
    height_ = rend.height;

    dispatcher.sink<wv::WindowChanged>().connect<&GameplayModule::window_changed>(*this);
    add_random_ship(100);
  }

  void update(entt::dispatcher& dispatcher, float dt) override {
    static float timer{0.0f};
    timer += dt;
    if (timer > 2) {
      timer = 0.0f;
      add_random_ship(1);
    }
    animate_sprites(dt);
    move_ships(dt);
  }

  void shutdown(entt::dispatcher& dispatcher) override {
    LOG_INFO("GameplayModule::shutdown");
    dispatcher.enqueue<wv::UnloadSpriteSheet>("ships");
    registry->clear();
  }

 private:
  void animate_sprites(float dt) {
    auto view = registry->view<wv::Sprite, wv::Animation>();
    for (auto [entity, sprite, anim] : view.each()) {
      anim.current += dt;
      if (anim.current > anim.frame_time) {
        anim.current = 0.0f;
        int range = anim.end - anim.start + 1;
        if (range <= 0) range = 1;
        sprite.frame = anim.start + ((sprite.frame + 1 - anim.start) % range);
      }
    }
  }
  float wrap_float(float value, float min, float max) {
    float range = max - min;
    if (range <= 0) return min;
    value -= min;
    value = fmodf(value, range);
    if (value < 0) {
      value += range;
    }
    return value + min;
  }
  static float lerp_angle(float a, float b, float t) {
    float diff = b - a;
    while (diff < -180.0f) {
      diff += 360.0f;
    }
    while (diff > 180.0f) {
      diff -= 360.0f;
    }
    return a + diff * t;
  }

  void move_ships(float dt) {
    auto view = registry->view<wv::Sprite, wv::Transform, wv::Velocity>();
    auto& rng = registry->ctx().get<Rngen>();

    for (auto [entity, sprite, transform, velocity] : view.each()) {
      velocity.x = std::lerp(velocity.x, velocity.target_x, 0.5f * dt);
      velocity.y = std::lerp(velocity.y, velocity.target_y, 0.5f * dt);

      transform.position.x += velocity.x * dt;
      transform.position.y += velocity.y * dt;

      float speed = std::hypot(velocity.x, velocity.y);
      if (speed > 1.0f) {
        float rot = std::atan2(velocity.y, velocity.x) * (180.0f / M_PI) - 90.0f;
        transform.rotation = lerp_angle(transform.rotation, rot, 1.5f * dt);
      }

      if (rng.random<float>(0.0f, 1.0f) < 0.002f) {
        float current = std::atan2(velocity.target_y, velocity.target_x);
        float new_target = current + rng.random<float>(-0.7854f, 0.7854f);

        float target_speed = rng.random<float>(40.0f, 80.0f);
        velocity.target_x = std::cos(new_target) * target_speed;
        velocity.target_y = std::sin(new_target) * target_speed;
      }

      transform.position.x = wrap_float(transform.position.x, 0.0f, static_cast<float>(width_));
      transform.position.y = wrap_float(transform.position.y, 0.0f, static_cast<float>(height_));
    }
  }

  void window_changed(wv::WindowChanged& ev) {
    width_ = ev.width;
    height_ = ev.height;
  }

  void add_random_ship(size_t count = 1) {
    for (size_t i = 0; i < count; i++) {
      auto entity = registry->create();
      registry->emplace<wv::Sprite>(entity, "ships", 0);
      registry->emplace<wv::Animation>(entity, 0, 4, 0.3f);

      auto transform = registry->ctx().get<Rngen>().random<wv::Transform>(width_, height_, 10, 50);
      transform.size = {66.0f, 113.0f};
      registry->emplace<wv::Transform>(entity, transform);

      auto color = registry->ctx().get<Rngen>().random<wv::Color>();
      color.a = 255;
      registry->emplace<wv::Color>(entity, color);

      float angle = registry->ctx().get<Rngen>().random<float>(0.0f, 2.0f * 3.14159265f);
      float speed = registry->ctx().get<Rngen>().random<float>(30.0f, 70.0f);
      float vx = std::cos(angle) * speed;
      float vy = std::sin(angle) * speed;
      registry->emplace<wv::Velocity>(entity, vx, vy, vx, vy);
    }
  }

  int width_;
  int height_;
};

WV_MODULE(GameplayModule)
