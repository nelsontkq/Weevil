
#include "WeevilEngine/SDLContext.h"

class SDLError : public std::runtime_error {
 public:
  explicit SDLError(const std::string &message) : std::runtime_error(message + ": " + SDL_GetError()) {}
};
wv::SDLContext::SDLContext(AppSettings &settings) {
  // Initialize SDL
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw SDLError("Failed to initialize SDL: " + std::string(SDL_GetError()));
  }
  auto flags = 0;
  if (settings.fullscreen) {
    flags |= SDL_WINDOW_BORDERLESS;
  } else if (settings.resizable) {
    flags |= SDL_WINDOW_RESIZABLE;
  }
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  if (!SDL_CreateWindowAndRenderer(settings.title.c_str(), settings.width, settings.height, flags, &window,
                                   &renderer)) {
    throw SDLError("Failed to create window and renderer");
  }

  if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)) {
    throw SDLError("Failed to set render draw color");
  }

  window_.reset(window);
  renderer_.reset(renderer);
}

wv::SDLContext::~SDLContext() { SDL_Quit(); }

SDL_Window *wv::SDLContext::get_window() const { return window_.get(); }

SDL_Renderer *wv::SDLContext::get_renderer() const { return renderer_.get(); }