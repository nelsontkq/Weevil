#pragma once

#include "weevil.h"
namespace wv {
class AssetManager {
 public:
  AssetManager(const AppSettings &app, SDL_Renderer *renderer);
  // Returns index of loaded asset. Assets will be cached until removed.
  auto load(const std::string &relative_path) -> uint64_t;
  // will unload an asset
  void unload(uint64_t asset_id);

  auto get(uint64_t asset_id) -> SDL_Texture *;

 private:
  SDL_Renderer *renderer_;
  std::unordered_map<uint64_t, SDL_Texture *> texture_map_;
  std::filesystem::path asset_path_;
  std::hash<std::string> hasher_;
};
}  // namespace wv
