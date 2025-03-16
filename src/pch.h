#pragma once

// Windows crap
#ifdef WV_PLATFORM_WINDOWS
#define NOMINMAX
#include <Windows.h>
#endif

// Standard Library Headers
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

// Third-Party Libraries
#include <SDL3/SDL.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <entt/entt.hpp>

// Engine
#include <weevil/core/app_settings.h>
#include <weevil/core/log.h>
#include <weevil/core/module.h>
#include <weevil/core/uuid.h>

#ifdef WV_ENABLE_ASSERTS
inline void wvAssertImpl(const bool condition, const char* conditionStr, const char* message) {
  if (condition) {
    return;
  }
  const auto error{"Assertion failed: " + std::string(conditionStr) + "\n" + message};
  // TODO: text too small on high dpi screen, use a custom dialog
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assertion Failed", error.c_str(), nullptr);
  abort();
}
inline void wvAssertImpl(const bool condition, const char* conditionStr, const std::string& message) {
  wvAssertImpl(condition, conditionStr, message.c_str());
}
#define WV_ASSERT(condition, message) wvAssertImpl((condition), #condition, (message))
#else
#define WV_ASSERT(condition, message) ((void)0)
#endif

const auto WV_MODULE_CREATOR_FN_NAME = "wv_create_module";

#ifdef WV_PLATFORM_WINDOWS
constexpr auto WV_DYLIB_EXT = ".dll";
#elif defined(WV_PLATFORM_LINUX)
constexpr auto WV_DYLIB_EXT = ".so";
#elif defined(WV_PLATFORM_MACOS)
constexpr auto WV_DYLIB_EXT = ".dylib";
#else
#error "Unsupported platform"
#endif