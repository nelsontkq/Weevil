//
// Created by nelson on 1/18/25.
//

#ifndef WEEVIL_INCLUDE_WEEVILENGINE_ASSETMANAGER_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_ASSETMANAGER_H_
#include "wvpch.h"

class AssetManager {
public:
  // Returns index of loaded asset. Assets will be cached until removed.
  auto load(std::filesystem::path) -> u_int64_t;
  // will unload an asset
  auto unload(uint64_t asset_id) -> void;

};

#endif //WEEVIL_INCLUDE_WEEVILENGINE_ASSETMANAGER_H_
