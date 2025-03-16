//
// Created by nelson on 2/1/25.
//

#include "application.h"

#include <weevil/core/app_settings.h>
#include <weevil/core/rngen.h>
#include <weevil/core/window.h>

#include "custom_events.h"
#include "pch.h"

wv::Application::Application()
    : settings_{},
      module_manager_(dispatcher_, settings_),
      asset_loader_(settings_.asset_path),
      platform_(&asset_loader_) {}

wv::Application* wv::Application::create(SDL_AppResult& out_result) {
  auto app = new wv::Application();
  out_result = app->init();
  return app;
}

SDL_AppResult wv::Application::init() {
  if (!platform_.init(settings_)) {
    return SDL_APP_FAILURE;
  }
  asset_loader_.register_dispatch_events(dispatcher_);
  module_manager_.init();
  return SDL_APP_CONTINUE;
}

SDL_AppResult wv::Application::process_event(SDL_Event& event) {
  if (event.type == SDL_EVENT_QUIT) {
    quitting_ = true;
  }
  dispatcher_.enqueue<SDL_Event&>(event);
  return SDL_APP_CONTINUE;
}

SDL_AppResult wv::Application::iterate() {
  dispatcher_.update();
  if (quitting_) {
    return SDL_APP_SUCCESS;
  }
  static auto last_ticks = SDL_GetTicks();
  auto current_ticks = SDL_GetTicks();
  auto delta_ticks = current_ticks - last_ticks;
  last_ticks = current_ticks;
  module_manager_.update((float)delta_ticks / 1000.f);
  platform_.render(module_manager_.registry());

  return SDL_APP_CONTINUE;
}
void wv::Application::shutdown() {
  module_manager_.shutdown();
  platform_.shutdown();
}
