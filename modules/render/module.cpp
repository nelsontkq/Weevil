#include <SDL3/SDL.h>
#include <weevil/weevil_api.h>

class RenderModule : public wv::IModule {
 public:
  void init(entt::registry& registry, entt::dispatcher& dispatcher) override {}
  void update(entt::registry& registry, entt::dispatcher& dispatcher, float dt) override {
    // Retrieve your rendering context (already in your code)
    auto ctx = registry.ctx().get<wv::Window>();

    // Clear screen with black
    SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
    SDL_RenderClear(ctx.renderer);

    // Draw all colored rectangles (already in your code)
    for (auto [entity, transform, color] : registry.view<const wv::Transform, const wv::Color>().each()) {
      SDL_FRect rect = {transform.position.x, transform.position.y, transform.size.width, transform.size.height};

      SDL_SetRenderDrawColor(ctx.renderer, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(ctx.renderer, &rect);
    }
    // Present everything on screen
    SDL_RenderPresent(ctx.renderer);
  }

  void shutdown(entt::registry& registry, entt::dispatcher& dispatcher) override { LOG_INFO("RenderModule::shutdown"); }
};
WV_MODULE(RenderModule)