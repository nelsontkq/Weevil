#pragma once

#include <string_view>
#include <toml++/toml.hpp>

#include "weevil.h"

namespace wv {

class AppSettings {
 public:
  AppSettings(const std::string_view& file_name);
  std::string title;
  std::string version;
  std::string app_identifier;
  int width;
  int height;
  bool fullscreen;
  bool resizable;
  std::string asset_path;
};

}  // namespace wv
