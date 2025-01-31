//
// Created by nelson on 2/1/25.
//
#include "WeevilEngine/AppContext.h"
#include "WeevilEngine/weevil.h"
wv::Application::Application(const wv::AppSettings& settings)
    : systems_(registry_) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    LOG_ERROR("Failed to initialize SDL" + std::string(SDL_GetError()));
    throw std::runtime_error("Failed to initialize SDL");
  }
  SDL_SetAppMetadata(settings.title.c_str(), settings.version.c_str(),
                     settings.app_identifier.c_str());
  auto flags = 0;
  if (settings.fullscreen) {
    flags |= SDL_WINDOW_BORDERLESS;
  } else if (settings.resizable) {
    flags |= SDL_WINDOW_RESIZABLE;
  }
  // SDL initializes these clang-tidy
  sdl_renderer_ = nullptr;
  sdl_window_ = nullptr;
  if (!SDL_CreateWindowAndRenderer(settings.title.c_str(), settings.width,
                                   settings.height, flags, &sdl_window_,
                                   &sdl_renderer_)) {
    LOG_ERROR("Failed to create window and renderer");
    throw std::runtime_error("Failed to initialize SDL");
  }
  if (!SDL_SetRenderDrawColor(sdl_renderer_, 0, 0, 0, SDL_MAX_UINT8)) {
    LOG_ERROR("Failed to set render draw color" + std::string(SDL_GetError()));
    throw std::runtime_error("Failed to initialize SDL");
  }
}
SDL_AppResult wv::Application::process_event(SDL_Event& event) {
  // TODO: keyboard events -> delegates
  return SDL_APP_CONTINUE;
}
void wv::Application::iterate() {
  const uint64_t currentTicks = SDL_GetTicks();
  delta_ticks_ = currentTicks - delta_ticks_;
  systems_.update(static_cast<float>(delta_ticks_) / 1000.0f);
  renderer_.render(sdl_renderer_, registry_);
}

void wv::Application::shutdown() {
  systems_.shutdown();
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
}
