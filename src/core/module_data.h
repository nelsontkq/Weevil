#pragma once
#include "pch.h"
namespace wv {
struct ModuleData {
  explicit ModuleData(const std::filesystem::path& filename) {
    auto base = std::filesystem::path(SDL_GetBasePath());
    std::error_code err;
    auto unique_name = std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + WV_DYLIB_EXT;
    auto output_path = base / "modules" / unique_name;
    std::filesystem::rename(base / "prebuild" / filename, output_path, err);
    if (err) {
      CORE_ERROR("Failed to rename file: {}", err.message());
      mod = new IModule();
      return;
    }
    so = SDL_LoadObject(output_path.c_str());
    if (!so) {
      CORE_ERROR("Failed to load module: {}", SDL_GetError());
      // dummy module to allow hot reloading
      mod = new IModule();
      return;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
    const auto create_module = (IModule * (*)()) SDL_LoadFunction(so, WV_MODULE_CREATOR_FN_NAME);
    mod = create_module();
    lib_path = SDL_strdup(output_path.c_str());
  }

  ~ModuleData() {
    if (lib_path) {
      std::filesystem::remove(lib_path);
      SDL_free(lib_path);
    }
    if (mod) {
      delete mod;
    }
    if (so) {
      SDL_UnloadObject(so);
    }
  }
  SDL_SharedObject* so;
  wv::IModule* mod;
  char* lib_path;
};
}  // namespace wv