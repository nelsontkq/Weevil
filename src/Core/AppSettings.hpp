
#pragma once
#include <nlohmann/json.hpp>

struct AppSettings
{
    static auto load() -> AppSettings;
    static auto load(const std::string &string) -> AppSettings;
    std::string title;
    int width;
    int height;
    bool fullscreen;
    std::filesystem::path asset_path = "";
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AppSettings, title, width, height, fullscreen, asset_path);