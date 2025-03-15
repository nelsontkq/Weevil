#include "text_texture.h"

wv::TextTexture::TextTexture(std::shared_ptr<TTF_Font> font)
    : font_(font), center_(nullptr), clip_(nullptr), flip_(SDL_FLIP_NONE) {}

void wv::TextTexture::render(SDL_Renderer* renderer, const wv::Transform& transform) {
  SDL_FRect t = transform;
  SDL_FRect* rect = nullptr;
  if (clip_ != nullptr) {
    t.w = clip_->rect.w;
    t.h = clip_->rect.h;
    rect = &clip_->rect;
  }

  SDL_RenderTextureRotated(renderer, texture_.get(), rect, &t, transform.rotation, center_, flip_);
}

void wv::TextTexture::set_font_size(float size) {
  if (!TTF_SetFontSize(font_.get(), size)) {
    LOG_ERROR("Failed to set font size: {}", SDL_GetError());
  }
}

void wv::TextTexture::set_text(SDL_Renderer* renderer, const std::string& text) {
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
}

void wv::TextTexture::set_clip(wv::Clip* clip) { clip_ = clip; }
