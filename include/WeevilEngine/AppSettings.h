#ifndef WV_APPSETTINGS_HPP
#define WV_APPSETTINGS_HPP

#include <string_view>
#include <toml++/toml.hpp>

#include "wvpch.h"

namespace wv {

class AppSettings {
 public:
  AppSettings(const std::filesystem::path& path);
  AppSettings();
  std::string title;
  int width;
  int height;
  bool fullscreen;
  bool resizable;
  std::string_view asset_path = "";
};

}  // namespace wv

#endif  // WV_APPSETTINGS_HPP
