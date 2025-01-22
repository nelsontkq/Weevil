//
// Created by nelson on 1/18/25.
//

#ifndef WEEVIL_INCLUDE_WEEVILENGINE_ASSETMANAGER_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_ASSETMANAGER_H_
#include "SDL_Deleter.h"
#include "wvpch.h"
namespace wv {
class AssetManager {
 public:
  AssetManager(AppSettings &app, SDL_Renderer *renderer);
  // Returns index of loaded asset. Assets will be cached until removed.
  uint64_t load(const std::string relative_path);
  // will unload an asset
  void unload(uint64_t asset_id);

  SDL_Texture *get(uint64_t asset_id);

 private:
  SDL_Renderer *renderer_;
  std::unordered_map<uint64_t, SDL_Texture *> texture_map_;
  std::filesystem::path asset_path_;
  std::hash<std::string> hasher_;
};
}  // namespace wv
#endif  // WEEVIL_INCLUDE_WEEVILENGINE_ASSETMANAGER_H_
