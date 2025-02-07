//
// Created by nelson on 2/1/25.
//
#include "WeevilEngine/AppContext.h"
#include "WeevilEngine/weevil.h"
wv::Application::Application(const wv::AppSettings& settings)
    : game_module_(new GameModule()) {
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
}
SDL_AppResult wv::Application::process_event(SDL_Event& event) {
  if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_F5) {
    LOG_INFO("F5 pressed, toggling hot reload");
    game_module_->trigger_reload();
  }
  return SDL_APP_CONTINUE;
}
SDL_AppResult wv::Application::iterate() {
  const uint64_t currentTicks = SDL_GetTicks();
  delta_ticks_ = currentTicks - delta_ticks_;
  if (game_module_ && game_module_->needs_reload()) {
    LOG_INFO("Hot reloading game module...");
    game_module_->shutdown();
    game_module_->unload();
    // Optionally, you might wait a moment or copy the file to a temp location.
    if (game_module_->load()) {
      game_module_->init(sdl_renderer_);
    } else {
      LOG_ERROR("Failed to reload game module.");
      return SDL_APP_FAILURE;
    }
  }
  if (game_module_) {
    game_module_->update(sdl_renderer_,
                         static_cast<float>(delta_ticks_) / 1000.f);
  }
}
void wv::Application::shutdown() {
  SDL_DestroyRenderer(sdl_renderer_);
  SDL_DestroyWindow(sdl_window_);
}
