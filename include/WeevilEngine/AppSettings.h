#ifndef WV_APPSETTINGS_HPP
#define WV_APPSETTINGS_HPP

#include <string_view>
#include <toml++/toml.hpp>
#include "NoCopy.h"

#include "wvpch.h"

namespace wv {

class AppSettings : NoCopy {
 public:
  AppSettings(const std::filesystem::path& path);
  std::string title;
  int width;
  int height;
  bool fullscreen;
  bool resizable;
  std::string asset_path;
};

}  // namespace wv

#endif  // WV_APPSETTINGS_HPP
