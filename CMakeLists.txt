cmake_minimum_required(VERSION 3.27)
project(Weevil LANGUAGES CXX)

# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.5/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=c46b876ae3b9f994b4f05a4c15553e0485636862064f1fcc9d8b4f832086bc5d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
## Dependencies
CPMAddPackage("gh:libsdl-org/SDL#prerelease-3.1.10")
CPMAddPackage("gh:libsdl-org/SDL_image#preview-3.1.0")
CPMAddPackage("gh:skypjack/entt@3.14.0")
CPMAddPackage("gh:gabime/spdlog@1.15.0")
CPMAddPackage("gh:marzer/tomlplusplus@3.4.0")

add_library(WeevilEngine STATIC
        src/Core/AppSettings.cpp
        src/Core/InputContext.cpp
        src/Core/Log.cpp
        src/Core/SDLContext.cpp
        src/Core/UUID.cpp
        src/Game/Game.cpp
        src/System/AssetManager.cpp
        src/System/SystemManager.cpp
        src/Scene/SceneManager.cpp
        include/WeevilEngine/SceneManager.h
        tests/brotato/MainGame/MainGameScene.cpp
        tests/brotato/MainGame/MainGameScene.h
)

add_library(Weevil::Engine ALIAS WeevilEngine)

# Set include directories for WeevilEngine
target_include_directories(WeevilEngine
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(WeevilEngine PUBLIC cxx_std_20)
target_compile_options(WeevilEngine PRIVATE -Wall -Wextra -Wpedantic -Wno-unknown-pragmas)

target_link_libraries(WeevilEngine
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        EnTT::EnTT
        spdlog::spdlog
        tomlplusplus::tomlplusplus
)


add_executable(WeevilTest tests/main.cpp tests/brotato/MainGame/PlayerSystem.cpp
        tests/brotato/MainGame/InputSystem.cpp
        tests/brotato/MainGame/InputSystem.h
        tests/brotato/BrotatoComponents.h
        tests/brotato/MainMenu/MainMenuScene.cpp
        tests/brotato/MainMenu/MainMenuScene.h)
target_link_libraries(WeevilTest PRIVATE Weevil::Engine
)
