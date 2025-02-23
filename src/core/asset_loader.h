#pragma once
#include "pch.h"
namespace wv {
enum class AssetType { FONT, TEXTURE, AUDIO };
class AssetLoader {
 public:
  // Mark an asset for loading.
  wv::UUID load_asset(std::filesystem::path asset_path, wv::AssetType type);
  bool is_loaded(wv::UUID asset_id);
  bool unload_asset(wv::UUID asset_id);
};
}  // namespace wv