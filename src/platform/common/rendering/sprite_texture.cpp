#include "sprite_texture.h"

wv::SpriteTexture::SpriteTexture(std::shared_ptr<SDL_Surface> surface) : surface_(surface), texture_(nullptr) {}

void wv::SpriteTexture::render(SDL_Renderer* renderer, const wv::Transform& transform) {
  if (!texture_ && surface_) {
    texture_.reset(SDL_CreateTextureFromSurface(renderer, surface_.get()));
    if (!texture_) {
      LOG_ERROR("Failed to create texture from surface: {}", SDL_GetError());
      return;
    }
  }

  SDL_FRect dest_rect = transform;
  SDL_FRect* src_rect = nullptr;
  if (!frames_.empty() && current_frame_ >= 0 && current_frame_ < frames_.size()) {
    src_rect = &frames_[current_frame_];
  }
  if (clip_) {
    src_rect = &clip_->rect;
  }
  SDL_SetTextureColorMod(texture_.get(), color_.r, color_.g, color_.b);
  SDL_SetTextureAlphaMod(texture_.get(), color_.a);
  SDL_RenderTextureRotated(renderer, texture_.get(), src_rect, &dest_rect, transform.rotation, center_, flip_);
}

void wv::SpriteTexture::set_frame(int frame_index) {
  if (frame_index >= 0 && frame_index < frames_.size()) {
    current_frame_ = frame_index;
  }
}

void wv::SpriteTexture::add_frame(const wv::Transform& frame) {
  SDL_FRect rect = frame;
  frames_.push_back(rect);
}

void wv::SpriteTexture::set_clip(wv::Clip* clip) { clip_ = clip; }

void wv::SpriteTexture::set_color(const wv::Color& color) { color_ = color; }
