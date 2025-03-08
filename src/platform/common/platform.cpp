#include "platform.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <weevil/core/components.h>

#include "rendering/text_texture.h"

wv::Platform::Platform() : sdl_renderer_(nullptr), sdl_window_(nullptr), asset_loader_(nullptr) {}

bool wv::Platform::init(wv::AppSettings& settings, wv::AssetLoader* asset_loader) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    CORE_ERROR("Failed to initialize SDL" + std::string(SDL_GetError()));
    return false;
  }
  SDL_SetAppMetadata(settings.title.c_str(), settings.version.c_str(), settings.app_identifier.c_str());
  auto flags = 0;
  if (settings.fullscreen) {
    flags |= SDL_WINDOW_BORDERLESS;
  } else if (settings.resizable) {
    flags |= SDL_WINDOW_RESIZABLE;
  }
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  if (!SDL_CreateWindowAndRenderer(settings.title.c_str(), settings.width, settings.height, flags, &window,
                                   &renderer)) {
    CORE_ERROR("Failed to create window and renderer");
    return false;
  }
  if (!SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_ADAPTIVE)) {
    CORE_WARN("Failed to set adaptive vsync {}", SDL_GetError());
    if (!SDL_SetRenderVSync(renderer, 1)) {
      CORE_ERROR("Failed to set vsync at all {}", SDL_GetError());
    }
  }
  if (!TTF_Init()) {
    CORE_ERROR("Failed to initialize TTF {}", SDL_GetError());
  }
  sdl_window_ = window;
  sdl_renderer_ = renderer;
  asset_loader_ = asset_loader;
  return true;
}

void wv::Platform::render(const std::vector<entt::registry*>& registries) {
  SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, 255);
  SDL_RenderClear(sdl_renderer_);
  for (const auto registry : registries) {
    for (const auto& [entity, transform, color] :
         registry->view<const wv::Rectangle, const wv::Transform, const wv::Color>().each()) {
      SDL_FRect rect = {transform.position.x, transform.position.y, transform.size.width, transform.size.height};

      SDL_SetRenderDrawColor(sdl_renderer_, color.r, color.g, color.b, color.a);
      SDL_RenderFillRect(sdl_renderer_, &rect);
    }
    for (const auto& [entity, transform, text] :
         registry->view<const wv::Transform, const wv::Text>().each()) {
      auto texture = asset_loader_->get(text.asset_name);
      const wv::Clip* clip = registry->try_get<wv::Clip>(entity);
      texture->render(sdl_renderer_, transform, text.value, clip);
    }
  }
  SDL_RenderPresent(sdl_renderer_);
}

void wv::Platform::shutdown() {
  TTF_Quit();
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
}
