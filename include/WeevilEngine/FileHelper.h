#include "wvpch.h"
#include "NoCopy.h"

namespace wv {
  constexpr char* CONFIG_FILE_NAME = "weevil.toml";
class FileHelper : NoCopy {
 public:
  static std::filesystem::path get_config(const std::string& app_name) {
    if (const std::filesystem::path env_path = std::getenv("WEEVIL_CONFIG_PATH")) {
      return env_path / CONFIG_FILE_NAME;
    }
    assert(!app_name.empty());
    auto* i = SDL_GetPrefPath("WeevilEngine", app_name.c_str());
    std::filesystem::path p(i);
    SDL_free(i);
    return p / CONFIG_FILE_NAME;
  }
}
}  // namespace wv