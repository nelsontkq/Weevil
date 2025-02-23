//
// Created by nelson on 2/1/25.
//

#include "application.h"

#include <weevil/core/app_settings.h>
#include <weevil/core/rngen.h>
#include <weevil/core/window.h>

#include "internal/custom_events.h"
#include "pch.h"

wv::Application::Application() : module_manager_(registry_, dispatcher_) {}

SDL_AppResult wv::Application::init() {
  if (!platform_.init(settings_)) {
    return SDL_APP_FAILURE;
  }
  init_registry();
  module_manager_.init(settings_);
  return SDL_APP_CONTINUE;
}

SDL_AppResult wv::Application::process_event(SDL_Event& event) {
  if (event.type == SDL_EVENT_QUIT) {
    quitting_ = true;
  }
  dispatcher_.trigger<SDL_Event&>(event);
  return SDL_APP_CONTINUE;
}

SDL_AppResult wv::Application::iterate() {
  if (quitting_) {
    return SDL_APP_SUCCESS;
  }
  static auto last_ticks = SDL_GetTicks();
  auto current_ticks = SDL_GetTicks();
  auto delta_ticks = current_ticks - last_ticks;
  last_ticks = current_ticks;
  module_manager_.update((float)delta_ticks / 1000.f);
  platform_.render(registry_);

  return SDL_APP_CONTINUE;
}
void wv::Application::shutdown() {
  module_manager_.shutdown();
  platform_.shutdown();
}

void wv::Application::init_registry() {
  registry_.ctx().emplace<wv::Window>(settings_.width, settings_.height);
  registry_.ctx().emplace<Rngen>();
}
