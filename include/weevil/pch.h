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
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

// Third-Party Libraries
#include <SDL3/SDL.h>

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

// Engine
#include "core/log.h"
#include "core/uuid.h"

// assertions

#ifdef WV_ENABLE_ASSERTS
inline void wvAssertImpl(const bool condition, const char* conditionStr,
                         const char* message) {
  if (condition) {
    return;
  }

  const auto error =
      "Assertion failed: " + std::string(conditionStr) + "\n" + message;
  // TODO: text too small, use a custom dialog
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Assertion Failed",
                           error.c_str(), nullptr);
  abort();
}
inline void wvAssertImpl(const bool condition, const char* conditionStr,
                         const std::string& message) {
  wvAssertImpl(condition, conditionStr, message.c_str());
}
#define WV_ASSERT(condition, message) \
  wvAssertImpl((condition), #condition, (message))
#else
#define WV_ASSERT(condition, message) ((void)0)
#endif
