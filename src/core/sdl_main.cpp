#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include "core/application.h"
extern "C" {

constexpr wv::AppSettings settings;
constexpr wv::Application context(settings);
/* This function runs once at startup. */
auto SDL_AppInit(void **appstate, int argc, char *argv[]) -> SDL_AppResult {
  wv::Log::Init();
  // TODO: setup a dev configuration otherwise load from preferred dir
  try {
    context = new wv::Application(settings);
  } catch (const std::exception &ex) {
    CORE_ERROR("Failed to initialize application: {}", ex.what());
    return SDL_APP_FAILURE;
  }
  return SDL_APP_CONTINUE;
}

auto SDL_AppEvent(void *appstate, SDL_Event *event) -> SDL_AppResult {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return context.process_event(*event);
}

/* This function runs once per frame, and is the heart of the program. */
auto SDL_AppIterate(void *appstate) -> SDL_AppResult {
  const auto context = static_cast<wv::Application *>(appstate);
  return context->iterate();
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  const auto context = static_cast<wv::Application *>(appstate);
  context->shutdown();
}
}
