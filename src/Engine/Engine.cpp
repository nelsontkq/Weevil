#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include <WeevilEngine/AppContext.h>
#include <WeevilEngine/Log.h>


/* This function runs once at startup. */
auto SDL_AppInit(void **appstate, int argc, char *argv[]) -> SDL_AppResult {
  wv::Log::Init();
  // TODO: setup a dev configuration otherwise load from preferred dir
  const auto settings = wv::AppSettings::load_from_file(
      "/home/nelson/Development/Weevil/config/weevil.toml");
  wv::Application* context = nullptr;
  try {
    context = new wv::Application(settings);
  } catch (const std::exception &) {
    return SDL_APP_FAILURE;
  }
  *appstate = context;
  return SDL_APP_CONTINUE;
}

auto SDL_AppEvent(void *appstate, SDL_Event *event) -> SDL_AppResult {
  const auto context = static_cast<wv::Application *>(appstate);
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  return context->process_event(*event);
}

/* This function runs once per frame, and is the heart of the program. */
auto SDL_AppIterate(void *appstate) -> SDL_AppResult {
  const auto context = static_cast<wv::Application *>(appstate);
  context->iterate();
  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  const auto context = static_cast<wv::Application *>(appstate);
  context->shutdown();
}