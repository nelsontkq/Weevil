#pragma once
#include <string>

#include "components.h"
#include "window.h"

namespace wv {
struct LoadFont {
  std::string asset_name;
  std::string asset_path;
  float font_size;
};
struct LoadSprite {
  std::string asset_name;
  std::string asset_path;
};
struct LoadSpriteSheet {
  std::string asset_name;
  std::string asset_path;
  std::vector<wv::Transform> sprites;
};
struct UnloadFont {
  std::string asset_name;
};
struct UnloadSprite {
  std::string asset_name;
};
struct UnloadSpriteSheet {
  std::string asset_name;
};

struct WindowChanged {
  int width;
  int height;
};
}  // namespace wv