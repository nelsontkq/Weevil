
#include "WeevilEngine/AppSettings.h"

#include <filesystem>
#include <fstream>

#include "WeevilEngine/wvpch.h"
namespace fs = std::filesystem;

wv::AppSettings::AppSettings(const std::filesystem::path& p) {
  toml::table table;
  try {
    table = toml::parse_file(p.u8string());
  } catch (const toml::parse_error& err) {
    LOG_ERROR("Failed to parse configuration at {0}: {1}", p.c_str(), err.description());
    throw err;
  }
  title = table["window"]["title"].value_or("Weevil Game");
  height = table["window"]["height"].value_or(1920);
  width = table["window"]["width"].value_or(1080);
  resizable = table["window"]["resizable"].value_or(false);
  fullscreen = table["window"]["fullscreen"].value_or(false);
  std::optional res = table["general"]["assets"].value<std::string_view>();
  if (!res.has_value()) {
    LOG_ERROR("Failed to load asset path from {0}", p.c_str());
    throw std::runtime_error("Failed to load asset path");
  }

  asset_path = *res;
  LOG_INFO("Loaded settings from {0}", p.c_str());
};