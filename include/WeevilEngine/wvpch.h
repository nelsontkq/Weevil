#pragma once

// platform detection
#ifdef _WIN32
#ifdef _WIN64
#define WV_PLATFORM_WINDOWS
#else
#error "x86 builds are not supported"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "iOS simulator is not supported"
#elif TARGET_OS_IPHONE == 1
#define WV_PLATFORM_IOS
#error "iOS is not supported"
#elif TARGET_OS_MAC == 1
#define WV_PLATFORM_MACOS
#error "MacOS is not supported"
#else
#error "Unknown Apple platform"
#endif
#elif defined(__ANDROID__)
#define WV_PLATFORM_ANDROID
#error "Android is not supported"
#elif defined(__linux__)
#define WV_PLATFORM_LINUX
#else
#error "Unknown platform!"alignas
#endif

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
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// Third-Party Libraries
#include <SDL3/SDL.h>

#include "entt/entt.hpp"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

// Engine
#include "AppSettings.h"
#include "Log.h"
#include "UUID.h"

// macros
#define BIT(x) (1 << x)

// assertions

inline void wvAssertImpl(bool condition, const char* conditionStr, const char* message) {
  if (condition) {
    return;
  }

  auto error = "Assertion failed: " + std::string(conditionStr) + "\n" + message;
  // TODO: text too small, use a custom dialog
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assertion Failed", error.c_str(), nullptr);
  abort();
}
inline void wvAssertImpl(bool condition, const char* conditionStr, const std::string& message) {
  wvAssertImpl(condition, conditionStr, message.c_str());
}
#if WV_ENABLE_ASSERTS
#define WV_ASSERT(condition, message) wvAssertImpl((condition), #condition, (message))
#else
#define WV_ASSERT(condition, message) ((void)0)
#endif

// templates

// derived type

template <typename TDerived, typename TBase>
concept Derived = std::is_base_of_v<TBase, TDerived>;

