#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <weevil/core/dispatchables.h>

#include "pch.h"
#include "platform/common/rendering/sprite_texture.h"
#include "platform/common/rendering/text_texture.h"
namespace wv {

class AssetLoader {
 public:
  AssetLoader(const std::string& asset_path);
  ~AssetLoader();
  // Mark an asset for loading.
  wv::TextTexture* get_text(std::string_view asset_name);
  wv::SpriteTexture* get_sprite(std::string_view asset_name);
  void register_dispatch_events(entt::dispatcher& dispatcher);

 private:
  void load_font(wv::LoadFont&);
  void unload_font(wv::UnloadFont&);
  void load_sprite_sheet(wv::LoadSpriteSheet&);
  void unload_sprite_sheet(wv::UnloadSpriteSheet&);
  std::unordered_map<std::string_view, std::unique_ptr<wv::TextTexture>> text_textures_;
  std::unordered_map<std::string_view, std::unique_ptr<wv::SpriteTexture>> sprite_textures_;
  std::unordered_map<std::string_view, std::shared_ptr<TTF_Font>> fonts_;
  std::unordered_map<std::string_view, std::shared_ptr<SDL_Surface>> surfaces_;
  std::filesystem::path asset_path_;
};
}  // namespace wv
