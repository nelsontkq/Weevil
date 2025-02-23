//
// Created by nelson on 2/1/25.
//

#include "application.h"

#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include "internal/custom_events.h"

SDL_AppResult wv::Application::init() {
  if  (!platform_.init(settings_)) {
    return SDL_APP_FAILURE;
  }
  // TODO: async
  module_manager_.init(settings_, platform_.renderer());
  return SDL_APP_CONTINUE;
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
  module_manager_.update(platform_.renderer(), (float)delta_ticks_ / 1000.f);

  return SDL_APP_CONTINUE;
}
void wv::Application::shutdown() {
  module_manager_.shutdown();
  platform_.shutdown();
}
