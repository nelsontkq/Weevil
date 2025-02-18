//
// Created by nelson on 2/1/25.
//

#include "application.h"

#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include "internal/custom_events.h"

void wv::Application::init() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    CORE_ERROR("Failed to initialize SDL" + std::string(SDL_GetError()));
    throw std::runtime_error("Failed to initialize SDL");
  }
  SDL_SetAppMetadata(settings_.title.c_str(), settings_.version.c_str(), settings_.app_identifier.c_str());
  auto flags = 0;
  if (settings_.fullscreen) {
    flags |= SDL_WINDOW_BORDERLESS;
  } else if (settings_.resizable) {
    flags |= SDL_WINDOW_RESIZABLE;
  }
  sdl_renderer_ = nullptr;
  sdl_window_ = nullptr;
  if (!SDL_CreateWindowAndRenderer(settings_.title.c_str(), settings_.width, settings_.height, flags, &sdl_window_,
                                   &sdl_renderer_)) {
    CORE_ERROR("Failed to create window and renderer");
    throw std::runtime_error("Failed to initialize SDL");
  }
  if (!SDL_SetRenderVSync(sdl_renderer_, SDL_RENDERER_VSYNC_ADAPTIVE)) {
    CORE_WARN("Failed to set adaptive vsync {}", SDL_GetError());
    if (!SDL_SetRenderVSync(sdl_renderer_, 1)) {
      CORE_ERROR("Failed to set vsync at all {}", SDL_GetError());
    }
  }
  // TODO: async
  module_manager_.init(settings_, sdl_renderer_);
}

SDL_AppResult wv::Application::process_event(SDL_Event& event) {
  if (event.type == SDL_EVENT_QUIT) {
    quitting_ = true;
  }
  module_manager_.process_system_event(event);
  return SDL_APP_CONTINUE;
}
SDL_AppResult wv::Application::iterate() {
  if (quitting_) {
    module_manager_.shutdown();
    return SDL_APP_SUCCESS;
  }
  const uint64_t currentTicks = SDL_GetTicks();
  delta_ticks_ = currentTicks - delta_ticks_;
  module_manager_.update(sdl_renderer_, (float)delta_ticks_ / 1000.f);

  return SDL_APP_CONTINUE;
}
void wv::Application::shutdown() {
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
}
