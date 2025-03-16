#pragma once
#include "pch.h"
namespace wv {
struct ModuleData {
  explicit ModuleData(const std::filesystem::path& filename) : so(nullptr), mod(nullptr) {
    auto base = std::filesystem::path(SDL_GetBasePath());
    std::error_code err;
    auto unique_name = std::to_string(std::chrono::system_clock::now().time_since_epoch().count()) + WV_DYLIB_EXT;
    lib_path = base / "modules" / unique_name;
    std::filesystem::rename(base / "prebuild" / filename, lib_path, err);
    if (err) {
      CORE_ERROR("Failed to rename file: {}", err.message());
      mod.reset(new Module());
      return;
    }
    so = SDL_LoadObject(lib_path.c_str());
    if (!so) {
      CORE_ERROR("Failed to load module: {}", SDL_GetError());
      mod.reset(new Module());
      return;
    }
    const auto create_module = (Module * (*)()) SDL_LoadFunction(so, WV_MODULE_CREATOR_FN_NAME);
    if (!create_module) {
      CORE_ERROR("Failed to find module creator function: {}", SDL_GetError());
      mod.reset(new Module());
      return;
    }
    mod.reset(create_module());
  }

  ~ModuleData() {
    std::filesystem::remove(lib_path);
    if (so) {
      SDL_UnloadObject(so);
    }
  }
  SDL_SharedObject* so;
  std::unique_ptr<wv::Module> mod;
  std::string lib_path;
};
}  // namespace wv
