#include "text_texture.h"

wv::TextTexture::TextTexture(std::shared_ptr<TTF_Font> font) : font_(font) {}

void wv::TextTexture::render(SDL_Renderer* renderer, const wv::Transform& transform, const std::string& text,
                             const wv::Clip* clip) {
  if (!surface_ || text != last_text_) {
    surface_.reset(TTF_RenderText_Blended(font_.get(), text.c_str(), 0, color_));
    if (!surface_) {
      LOG_ERROR("Failed to create surface: {}", SDL_GetError());
    }
    texture_.reset(SDL_CreateTextureFromSurface(renderer, surface_.get()));
    if (!texture_) {
      LOG_ERROR("Failed to create texture: {}", SDL_GetError());
    }
  }
  SDL_FRect t = transform;
  if (clip != nullptr) {
    t.w = clip->size.width;
    t.h = clip->size.height;
  }
  SDL_RenderTextureRotated(renderer, texture_.get(), (SDL_FRect*)clip, &t, angle_, center_, flip_);
}

void wv::TextTexture::set_font_size(float size) { TTF_SetFontSize(font_.get(), size); }
