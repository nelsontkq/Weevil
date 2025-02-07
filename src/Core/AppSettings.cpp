#include "WeevilEngine/AppSettings.h"

#include "WeevilEngine/weevil.h"
namespace fs = std::filesystem;

wv::AppSettings::AppSettings(const std::string_view& file_name) {
  toml::table table;
  try {
    table = toml::parse_file(file_name);
  } catch (const toml::parse_error& err) {
    LOG_ERROR("Failed to parse configuration at {0}: {1}", file_name,
              err.description());
    throw err;
  }
  title = table["window"]["title"].value_or("Weevil Game");
  height = table["window"]["height"].value_or(1920);
  width = table["window"]["width"].value_or(1080);
  resizable = table["window"]["resizable"].value_or(false);
  fullscreen = table["window"]["fullscreen"].value_or(false);
  version = table["general"]["version"].value_or("0.0.1");
  app_identifier =
      table["general"]["app_identifier"].value_or("com.weevil.game");
  const std::optional res =
      table["general"]["assets"].value<std::string_view>();
  if (!res.has_value()) {
    LOG_ERROR("Failed to load asset path from {0}", file_name);
    throw std::runtime_error("Failed to load asset path");
  }
  asset_path = *res;
  LOG_INFO("Loaded settings from {0}", file_name);
};