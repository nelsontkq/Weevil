#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <core/sdl_deleter.h>
#include <weevil/core/components.h>

#include "pch.h"
namespace wv {
class TextTexture {
 public:
  explicit TextTexture(std::shared_ptr<TTF_Font> font);

  void render(SDL_Renderer* renderer, const wv::Transform& transform, const std::string& text, const wv::Clip* clip);
  void set_font_size(float size);

 private:
  std::shared_ptr<TTF_Font> font_;
  std::string last_text_;
  std::unique_ptr<SDL_Surface, wv::SDL_Deleter> surface_;
  std::unique_ptr<SDL_Texture, wv::SDL_Deleter> texture_;
  SDL_FPoint* center_ = nullptr;
  SDL_FlipMode flip_ = SDL_FLIP_NONE;
  double angle_ = 0;
  wv::Color color_ = {0xFF, 0xFF, 0xFF, 0xFF};
};
}  // namespace wv