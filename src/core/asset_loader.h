#pragma once
#include <SDL3_ttf/SDL_ttf.h>
#include <weevil/core/dispatchables.h>

#include "pch.h"
namespace wv {
class AssetLoader {
 public:
  AssetLoader(const std::string asset_path);
  ~AssetLoader();
  // Mark an asset for loading.
  void load_font(wv::LoadFont& asset);
  bool unload_asset(std::string& asset_name);

 private:
  entt::dense_map<std::string_view, TTF_Font*> fonts_;
  std::filesystem::path asset_path_;
};
}  // namespace wv