#pragma once

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
  std::string src_dir;
  std::string debug_preset; 
};

}  // namespace wv
