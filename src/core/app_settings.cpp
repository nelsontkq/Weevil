#include <weevil/core/app_settings.h>
#include <weevil/pch.h>

#include <toml++/toml.hpp>

wv::AppSettings::AppSettings() {
  const auto exe_dir = SDL_GetBasePath();
  const std::string file_name = "app_settings.toml";

  toml::table table;
  try {
    table = toml::parse_file(file_name);
  } catch (const toml::parse_error& err) {
    LOG_ERROR("Failed to parse configuration at {0}: {1}", file_name, err.description());
    throw err;
  }
  title = table["window"]["title"].value_or("Weevil Game");
  height = table["window"]["height"].value_or(1920);
  width = table["window"]["width"].value_or(1080);
  resizable = table["window"]["resizable"].value_or(false);
  fullscreen = table["window"]["fullscreen"].value_or(false);
  version = table["general"]["version"].value_or("0.0.1");
  app_identifier = table["general"]["app_identifier"].value_or("com.weevil.game");
  const std::optional res = table["general"]["src_dir"].value<std::string_view>();

  if (!res.has_value()) {
    LOG_ERROR("Failed to load modules directory from {0}", file_name);
    throw std::runtime_error("Failed to load modules directory");
  }
  src_dir = *res;

  debug_preset = table["general"]["debug_preset"].value_or("linux-debug");

  LOG_INFO("Loaded settings from {0}", file_name);
}