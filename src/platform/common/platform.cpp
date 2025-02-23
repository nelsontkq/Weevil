#include "platform.h"

wv::Platform::Platform() : sdl_renderer_(nullptr), sdl_window_(nullptr) {}

bool wv::Platform::init(wv::AppSettings& settings) {
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
  sdl_window_ = window;
  sdl_renderer_ = renderer;
  return true;
}

void wv::Platform::shutdown() {
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
}
