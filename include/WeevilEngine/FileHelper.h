#include "NoCopy.h"
#include "weevil.h"

namespace wv {
constexpr char* CONFIG_FILE_NAME = "weevil.toml";
class FileHelper : NoCopy {
 public:
  static std::filesystem::path get_config(const std::string& app_name) {
    if (const std::filesystem::path env_path = std::getenv("WEEVIL_CONFIG_PATH")) {
      auto p = env_path / CONFIG_FILE_NAME;
      WV_ASSERT(std::filesystem::exists(p), "Config file " << p << " does not exist");
      return p;
    }
    WV_ASSERT(!app_name.empty(), "App name cannot be empty");
    auto* i = SDL_GetPrefPath("WeevilEngine", app_name.c_str());
    std::filesystem::path p(i);
    SDL_free(i);
    return p / CONFIG_FILE_NAME;
  }
}
}  // namespace wv