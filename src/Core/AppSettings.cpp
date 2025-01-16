#include "AppSettings.hpp"
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

auto AppSettings::load() -> AppSettings
{
    fs::path current_dir = fs::current_path();

    while (true)
    {
        if (fs::path potential_path = current_dir / "weevil.json"; exists(potential_path))
        {
            return AppSettings::load(potential_path);
        }

        if (current_dir == current_dir.root_path())
        {
            break;
        }

        current_dir = current_dir.parent_path();
    }
    AppSettings settings;
    return settings;
}
auto AppSettings::load(const std::string &string) -> AppSettings
{
    std::ifstream file(string);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + string);
    }
    nlohmann::json j;
    file >> j;
    return j.get<AppSettings>();
}