#ifndef WV_APPSETTINGS_HPP
#define WV_APPSETTINGS_HPP
#include <nlohmann/json.hpp>
namespace wv
{
    class AppSettings
    {
    public:
        static auto load() -> AppSettings;
        static auto load(const std::string &string) -> AppSettings;
        std::string title;
        int width;
        int height;
        bool fullscreen;
        bool keep_aspect_ratio;
        std::filesystem::path asset_path = "";
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AppSettings, title, width, height, fullscreen, keep_aspect_ratio, asset_path);
}
