#include "asset_loader.h"

#include <SDL3_ttf/SDL_ttf.h>

wv::AssetLoader::AssetLoader(const std::string asset_path) : asset_path_(asset_path) {}

wv::AssetLoader::~AssetLoader() {
  for (const auto& [name, font] : fonts_) {
    TTF_CloseFont(font);
  }
  fonts_.clear();
}

void wv::AssetLoader::load_font(wv::LoadFont& asset) {
  auto p = asset_path_ / asset.asset_path;
  if (!std::filesystem::exists(p)) {
    LOG_ERROR("Asset not found: {}", asset.asset_path);
    return;
  }
  LOG_INFO("Loading font: {}", asset.asset_path);
  if (fonts_.contains(asset.asset_name)) {
    LOG_INFO("Font already loaded: {}", asset.asset_name);
    return;
  }
  auto* font = TTF_OpenFont(asset.asset_path.c_str(), asset.font_size);
  if (font == nullptr) {
    LOG_ERROR("Failed to load font: {}", SDL_GetError());
    return;
  }
  fonts_.emplace(asset.asset_name, font);
  LOG_INFO("Font loaded: {}", asset.asset_name);
}

bool wv::AssetLoader::unload_asset(std::string& asset_name) {
  auto it = fonts_.find(asset_name);
  if (it == fonts_.end()) {
    // it's fine to call unload multiple times
    return false;
  }
  TTF_CloseFont(it->second);
  fonts_.erase(it);
  return true;
}