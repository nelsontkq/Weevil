cmake_minimum_required(VERSION 3.30)
project(Weevil LANGUAGES CXX)

find_package(SDL3 REQUIRED)
add_subdirectory(vendor)

option(WV_HOT_RELOAD "Enable hot reload" OFF)
option(WV_ENABLE_ASSERTS "Enable asserts" OFF)
set(WV_LOG "info" CACHE STRING "Log level")
set_property(CACHE WV_LOG PROPERTY STRINGS trace info warn error none)

add_compile_definitions(WV_CODE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_compile_definitions(WV_HOT_RELOAD=${WV_HOT_RELOAD})
add_compile_definitions(WV_ENABLE_ASSERTS=${WV_ENABLE_ASSERTS})

if(WV_LOG STREQUAL "trace")
        add_compile_definitions(WV_LOG_TRACE=1)
elseif(WV_LOG STREQUAL "info")
        add_compile_definitions(WV_LOG_INFO=1)
elseif(WV_LOG STREQUAL "warn")
        add_compile_definitions(WV_LOG_WARN=1)
elseif(WV_LOG STREQUAL "error")
        add_compile_definitions(WV_LOG_ERROR=1)
elseif(WV_LOG STREQUAL "none")
        add_compile_definitions(WV_LOG_NONE=1)
else()
        message(FATAL_ERROR "Invalid log level ${WV_LOG}")
endif()

if(WIN32)
        add_compile_definitions(WV_PLATFORM_WINDOWS=1)
        set(PLATFORM_NAME "windows")
elseif(APPLE)
        add_compile_definitions(WV_PLATFORM_APPLE=1)
        set(PLATFORM_NAME "apple")
elseif(UNIX)
        # Assume support for non-linux unix platforms rather than erroring out
        add_compile_definitions(WV_PLATFORM_LINUX=1)
        set(PLATFORM_NAME "linux")
else()
        message(FATAL_ERROR "Unsupported platform")
endif()

add_library(WeevilEngine SHARED
        src/core/app_settings.cpp
        src/core/application.cpp
        src/core/asset_loader.cpp
        src/core/module_manager.cpp
        src/core/sdl_main.cpp
        src/core/uuid.cpp
        src/platform/common/platform.cpp
        src/platform/common/rendering/text_texture.cpp
)

if(NOT WV_LOG STREQUAL "none")
        target_sources(WeevilEngine PRIVATE
                src/core/log.cpp
        )
endif()

if(WV_HOT_RELOAD)
        target_sources(WeevilEngine PRIVATE
                src/platform/${PLATFORM_NAME}/hot_reloader.cpp
        )
endif()

target_precompile_headers(WeevilEngine
        PRIVATE
        src/pch.h
)

target_include_directories(WeevilEngine
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_features(WeevilEngine PUBLIC cxx_std_20)

target_compile_options(WeevilEngine PRIVATE -Wall -Wextra -Wpedantic -Wno-unknown-pragmas)
target_link_libraries(WeevilEngine
        PUBLIC
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3::SDL3
        spdlog::spdlog
        tomlplusplus::tomlplusplus
        EnTT::EnTT
)

add_executable(WeevilApp src/dummy.cpp)
target_link_libraries(WeevilApp PRIVATE WeevilEngine)

# modules
add_subdirectory(modules)

configure_file(app_settings.toml app_settings.toml COPYONLY)
