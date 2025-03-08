#include "asset_loader.h"
struct FontDeleter {
  void operator()(TTF_Font* f) const { TTF_CloseFont(f); }
};
wv::AssetLoader::AssetLoader(const std::string asset_path) : asset_path_(asset_path) {}

wv::AssetLoader::~AssetLoader() {
  textures_.clear();
  fonts_.clear();
}
void wv::AssetLoader::load_font(wv::LoadFont& font_details) {
  auto p = asset_path_ / font_details.asset_path;
  if (!std::filesystem::exists(p)) {
    LOG_ERROR("Asset not found: {}", font_details.asset_path);
    return;
  }
  auto it = fonts_.find(font_details.asset_name);
  std::shared_ptr<TTF_Font> font;
  if (it != fonts_.end()) {
    font = it->second;
  } else {
    font = std::shared_ptr<TTF_Font>(TTF_OpenFont(p.c_str(), font_details.font_size), FontDeleter());
    if (!font) {
      LOG_ERROR("Failed to load font: {}", SDL_GetError());
      return;
    }
    fonts_[font_details.asset_name] = font;
  }

  const auto& [_, success] = textures_.emplace(font_details.asset_name, std::make_unique<wv::TextTexture>(font));
  WV_ASSERT(success, std::format("Texture for font {} being reinitialized", font_details.asset_name));

  LOG_INFO("Font loaded: {}", font_details.asset_name);
}

wv::TextTexture* wv::AssetLoader::get(std::string_view asset_name) {
  auto it = textures_.find(asset_name);
  WV_ASSERT(it != textures_.end(), "Attempted to get an unloaded text!");
  return it->second.get();
}

bool wv::AssetLoader::unload_font(wv::UnloadFont& asset) {
  auto it = textures_.find(asset.asset_name);
  if (it == textures_.end()) {
    return false;
  }
  textures_.erase(it);
  auto font_it = fonts_.find(asset.asset_name);
  if (font_it->second.use_count() == 1) {
    fonts_.erase(font_it);
  }
  return true;
}
