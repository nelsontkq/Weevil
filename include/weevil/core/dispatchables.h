#pragma once
#include <string>

namespace wv {
struct LoadFont {
  std::string asset_name;
  std::string asset_path;
  float font_size;
};
struct UnloadFont {
  std::string asset_name;
};
}  // namespace wv