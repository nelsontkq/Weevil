#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <core/sdl_deleter.h>
#include <weevil/core/components.h>

#include "pch.h"
#include "texture.h"

namespace wv {
class TextTexture : public wv::Texture {
 public:
  explicit TextTexture(std::shared_ptr<TTF_Font> font);

  void render(SDL_Renderer* renderer, const wv::Transform& transform) override;
  void set_font_size(float size);
  void set_text(SDL_Renderer* renderer, const std::string& text);
  void set_clip(wv::Clip* clip);

 private:
  std::shared_ptr<TTF_Font> font_;
  std::string last_text_;
  std::unique_ptr<SDL_Surface, wv::SDL_Deleter> surface_;
  std::unique_ptr<SDL_Texture, wv::SDL_Deleter> texture_;
  SDL_FPoint* center_;
  SDL_FlipMode flip_;
  wv::Clip* clip_;
  wv::Color color_ = {0xFF, 0xFF, 0xFF, 0xFF};
};
}  // namespace wv