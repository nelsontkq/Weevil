#pragma once

#include <string_view>
#include <toml++/toml.hpp>

#include <weevil/pch.h>

namespace wv {

class AppSettings {
 public:
  AppSettings();
  std::string title;
  std::string version;
  std::string app_identifier;
  int width;
  int height;
  bool fullscreen;
  bool resizable;
  std::string asset_path;
  // Development settings
  std::string module_path;
  std::string cmake_build_args;
};

}  // namespace wv
