#include "WeevilEngine/AppSettings.h"
#include "WeevilEngine/weevil.h"
namespace fs = std::filesystem;

auto wv::AppSettings::load_from_file(const std::string_view& file_name) -> wv::AppSettings&& {
  toml::table table;
  try {
    table = toml::parse_file(file_name);
  } catch (const toml::parse_error& err) {
    LOG_ERROR("Failed to parse configuration at {0}: {1}", file_name,
              err.description());
    throw err;
  }
  AppSettings settings;
  settings.title = table["window"]["title"].value_or("Weevil Game");
  settings.height = table["window"]["height"].value_or(1920);
  settings.width = table["window"]["width"].value_or(1080);
  settings.resizable = table["window"]["resizable"].value_or(false);
  settings.fullscreen = table["window"]["fullscreen"].value_or(false);
  settings.version = table["general"]["version"].value_or("0.0.1");
  settings.app_identifier =
      table["general"]["app_identifier"].value_or("com.weevil.game");
  const std::optional res =
      table["general"]["assets"].value<std::string_view>();
  if (!res.has_value()) {
    LOG_ERROR("Failed to load asset path from {0}", file_name);
    throw std::runtime_error("Failed to load asset path");
  }

  settings.asset_path = *res;
  LOG_INFO("Loaded settings from {0}", file_name);
  return std::move(settings);
};