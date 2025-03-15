#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <weevil/core/app_settings.h>

#include "core/application.h"
#include "pch.h"
extern "C" {

/* This function runs once at startup. */
auto SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> SDL_AppResult {
  wv::Log::Init();
  SDL_AppResult result;
  *appstate = wv::Application::create(result);
  return result;
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
void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result) {
  const auto context = reinterpret_cast<wv::Application *>(appstate);
  context->shutdown();
}
}
