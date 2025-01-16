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
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <cstdint>
#include <stdexcept>
#include <random>

// Third-Party Libraries
#include <cassert>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>


#include "Core/Log.hpp"

// macros
#define BIT(x) (1 << x)
