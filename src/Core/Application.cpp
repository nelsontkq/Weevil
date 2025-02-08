//
// Created by nelson on 2/1/25.
//
#include "AppContext.h"
#include "CustomEvents.h"
#include "WeevilEngine/weevil.h"

wv::Application::Application(const wv::AppSettings& settings) {
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
  // Vsync
  if (!SDL_SetRenderVSync(sdl_renderer_, SDL_RENDERER_VSYNC_ADAPTIVE)) {
    LOG_WARN("Failed to set adaptive vsync {}", SDL_GetError());
    if (!SDL_SetRenderVSync(sdl_renderer_, 1)) {
      LOG_ERROR("Failed to set vsync at all {}", SDL_GetError());
    }
  }
#ifdef WV_HOT_RELOAD
  file_watcher_ = new FileWatcher(settings.module_path, settings.build_command);
  file_watcher_->start();
  SDL_Event reload;
  reload.type = SDL_EVENT_USER;
  reload.user.code = WV_EVENT_RELOAD_MODULE;
  SDL_PushEvent(&reload);
#endif
}
SDL_AppResult wv::Application::process_event(SDL_Event& event) {
  if (event.type == SDL_EVENT_USER) {
    switch (event.user.code) {
      case WV_EVENT_RELOAD_MODULE:
        game_module_.trigger_reload(sdl_renderer_);
        break;
      case WV_EVENT_REBUILD_MODULE:
        file_watcher_->run_build_command();
        break;
      default:
        break;
    }
  }
  return SDL_APP_CONTINUE;
}
SDL_AppResult wv::Application::iterate() {
  const uint64_t currentTicks = SDL_GetTicks();
  delta_ticks_ = currentTicks - delta_ticks_;
  game_module_.update(sdl_renderer_, static_cast<float>(delta_ticks_) / 1000.f);

  return SDL_APP_CONTINUE;
}
void wv::Application::shutdown() {
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
}
