#pragma once
#include <string>

namespace wv {
struct LoadFont {
  std::string_view asset_name;
  std::string asset_path;
  float font_size;
};
}  // namespace wv