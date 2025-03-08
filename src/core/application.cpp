//
// Created by nelson on 2/1/25.
//

#include "application.h"

#include <weevil/core/app_settings.h>
#include <weevil/core/rngen.h>
#include <weevil/core/window.h>

#include "custom_events.h"
#include "pch.h"

wv::Application::Application() : settings_(), module_manager_(dispatcher_, settings_), asset_loader_(settings_.asset_path) {}

SDL_AppResult wv::Application::init() {
  if (!platform_.init(settings_, &asset_loader_)) {
    return SDL_APP_FAILURE;
  }
  module_manager_.init();
  dispatcher_.sink<wv::LoadFont>().connect<&AssetLoader::load_font>(&asset_loader_);
  dispatcher_.sink<wv::UnloadFont>().connect<&AssetLoader::unload_font>(&asset_loader_);
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
  auto& registries = module_manager_.update((float)delta_ticks / 1000.f);
  platform_.render(registries);

  return SDL_APP_CONTINUE;
}
void wv::Application::shutdown() {
  module_manager_.shutdown();
  platform_.shutdown();
}
