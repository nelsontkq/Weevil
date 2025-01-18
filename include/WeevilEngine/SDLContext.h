#ifndef WV_SDLCONTEXT_HPP
#define WV_SDLCONTEXT_HPP

#include "wvpch.h"
#include "Utils/SDL_Deleter.hpp"
#include "AppSettings.h"

namespace wv {
// SDLContext class
class SDLContext {
 public:
  explicit SDLContext(AppSettings &settings);

  ~SDLContext();

  [[nodiscard]] SDL_Window *get_window() const;

  [[nodiscard]] SDL_Renderer *get_renderer() const;

 private:
  std::unique_ptr<SDL_Window, SDL_Deleter> window_;
  std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
};
}
#endif