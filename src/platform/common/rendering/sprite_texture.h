#pragma once
#include <core/sdl_deleter.h>
#include <weevil/core/components.h>

#include "pch.h"
#include "texture.h"

namespace wv {
class SpriteTexture : public wv::Texture {
 public:
  explicit SpriteTexture(std::shared_ptr<SDL_Surface> surface);
  void render(SDL_Renderer* renderer, const wv::Transform& transform) override;
  void set_frame(int frame_index);
  void add_frame(const wv::Transform& frame);
  void set_clip(wv::Clip* clip);
  void set_color(const wv::Color& color);
  void set_rotation(float degrees);

 private:
  std::shared_ptr<SDL_Surface> surface_;
  std::unique_ptr<SDL_Texture, wv::SDL_Deleter> texture_;
  std::vector<SDL_FRect> frames_;
  int current_frame_{0};
  SDL_FPoint* center_{nullptr};
  SDL_FlipMode flip_{SDL_FLIP_NONE};
  wv::Clip* clip_{nullptr};
  wv::Color color_{0xFF, 0xFF, 0xFF, 0xFF};
};
}  // namespace wv
