//
// Created by nelson on 1/20/25.
//
#include "WeevilEngine/AssetManager.h"
#include <SDL3_image/SDL_image.h>
namespace wv {
AssetManager::AssetManager(const AppSettings &app, SDL_Renderer *renderer)
    : renderer_(renderer), asset_path_(app.asset_path) {}
auto AssetManager::get(const uint64_t asset_id) -> SDL_Texture * {
  return texture_map_[asset_id];
}

// Load an asset and return the key.
auto AssetManager::load(const std::string &relative_path) -> uint64_t {
  const uint64_t key = hasher_(relative_path);
  if (texture_map_.contains(key)) {
    return key;
  }
  const auto p = asset_path_ / relative_path;
  WV_ASSERT(exists(p), "Asset does not exist: " + p.string());
  LOG_INFO("Loading asset: {}", relative_path.c_str());
  // TODO: use SDL_AsyncIOFromFile
  auto *texture = IMG_LoadTexture(renderer_, p.c_str());
  if (texture == nullptr) {
    LOG_ERROR("Failed to load asset: {}", SDL_GetError());
    return 0;
  }

  texture_map_[key] = texture;
  return key;
}

// Unload an asset by key. NOTE: This is not thread safe and relatively
// expensive. It's best to keep the assets loaded if possible.
auto AssetManager::unload(uint64_t asset_id) -> void {
  if (const auto it = texture_map_.find(asset_id); it != texture_map_.end()) {
    LOG_INFO("Unloading asset: {}", asset_id);
    SDL_DestroyTexture(it->second);
    texture_map_.erase(it);
  }
}
}  // namespace wv
