#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <weevil/core/app_settings.h>
#include "pch.h"

#include "core/application.h"
extern "C" {

/* This function runs once at startup. */
auto SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) -> SDL_AppResult {
  try {
    wv::Application *context = nullptr;
    wv::Log::Init();
    context = new wv::Application();
    const auto result = context->init();
    *appstate = context;
    return result;
  } catch (const std::exception &ex) {
    CORE_ERROR("Failed to initialize application: {}", ex.what());
    return SDL_APP_FAILURE;
  }
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
