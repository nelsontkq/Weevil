#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <weevil/core/dispatchables.h>

#include "pch.h"
#include "platform/common/rendering/text_texture.h"
namespace wv {

class AssetLoader {
 public:
  AssetLoader(const std::string asset_path);
  ~AssetLoader();
  // Mark an asset for loading.
  void load_font(wv::LoadFont& asset);
  wv::TextTexture* get(std::string_view asset_name);
  bool unload_font(wv::UnloadFont& asset);

 private:
  std::unordered_map<std::string_view, std::unique_ptr<wv::TextTexture>> textures_;
  std::unordered_map<std::string_view, std::shared_ptr<TTF_Font>> fonts_;
  std::unordered_map<std::string_view, int> font_usage_counts_;
  std::filesystem::path asset_path_;
};
}  // namespace wv