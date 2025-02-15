#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include "core/application.h"
extern "C" {

/* This function runs once at startup. */
auto SDL_AppInit(void **appstate, int argc, char *argv[]) -> SDL_AppResult {
  wv::Log::Init();
  wv::Application *context = nullptr;
  try {
    context = new wv::Application();
    context->init();
  } catch (const std::exception &ex) {
    CORE_ERROR("Failed to initialize application: {}", ex.what());
    return SDL_APP_FAILURE;
  }
  *appstate = context;
  return SDL_APP_CONTINUE;
}

auto SDL_AppEvent(void *appstate, SDL_Event *event) -> SDL_AppResult {
  const auto context = reinterpret_cast<wv::Application *>(appstate);
  return context->process_event(*event);
}

/* This function runs once per frame, and is the heart of the program. */
auto SDL_AppIterate(void *appstate) -> SDL_AppResult {
  const auto context = reinterpret_cast<wv::Application *>(appstate);
  return context->iterate();
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  const auto context = reinterpret_cast<wv::Application *>(appstate);
  context->shutdown();
}
}
