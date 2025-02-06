cmake_minimum_required(VERSION 3.30)
project(Weevil LANGUAGES CXX)

add_subdirectory(vendor)

add_library(WeevilEngine STATIC
        src/Core/AppSettings.cpp
        src/Core/Log.cpp
        src/Core/UUID.cpp
        src/System/AssetManager.cpp
        src/System/SystemManager.cpp
        src/Renderer/Renderer.cpp
        src/Core/Application.cpp
        src/Engine/Engine.cpp
        include/WeevilEngine/Engine.h
)

target_precompile_headers(WeevilEngine
        PRIVATE
        include/WeevilEngine/weevil.h
)
add_library(Weevil::Engine ALIAS WeevilEngine)

# Set include directories for WeevilEngine
target_include_directories(WeevilEngine
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_compile_features(WeevilEngine PUBLIC cxx_std_20)
target_compile_options(WeevilEngine PRIVATE -Wall -Wextra -Wpedantic -Wno-unknown-pragmas)

target_link_libraries(WeevilEngine
        PUBLIC
        SDL3_image::SDL3_image
        SDL3::SDL3
        EnTT::EnTT
        spdlog::spdlog
        tomlplusplus::tomlplusplus
)

add_executable(WeevilTest
        Game/main.cpp)


target_include_directories(WeevilTest PRIVATE Game)
target_link_libraries(WeevilTest PRIVATE Weevil::Engine)
