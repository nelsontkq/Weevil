#ifndef WV_SDLCONTEXT_HPP
#define WV_SDLCONTEXT_HPP

#include "SDL_Deleter.h"
#include "wvpch.h"

namespace wv {

class SDLError : public std::runtime_error, NoCopy {
 public:
  explicit SDLError(const std::string &message) : std::runtime_error(message + ": " + SDL_GetError()) {}
};
// SDLContext class
class SDLContext {
 public:
  explicit SDLContext(AppSettings &settings);
  SDL_Window *get_window() const;
  SDL_Renderer *get_renderer() const;
  ~SDLContext();

 private:
  std::unique_ptr<SDL_Window, SDL_Deleter> window_;
  std::unique_ptr<SDL_Renderer, SDL_Deleter> renderer_;
};
}  // namespace wv
#endif