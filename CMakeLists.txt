cmake_minimum_required(VERSION 3.30)
project(Weevil LANGUAGES CXX)

find_package(SDL3 REQUIRED)
add_subdirectory(vendor)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_compile_definitions(WV_HOT_RELOAD=1)
        add_compile_definitions(WV_LOG_LEVEL=${WV_LOG_LEVEL})
        add_compile_definitions(WV_ENABLE_ASSERTS=1)
else()
        set(WV_HOT_RELOAD OFF)
        set(WV_LOG_LEVEL 2)
endif()

add_library(WeevilEngine SHARED
        src/Core/Application.cpp
        src/Core/AppSettings.cpp
        src/Core/GameModule.cpp
        src/Core/Log.cpp
        src/Core/UUID.cpp
        src/HotReloading/FileWatcher.cpp
        src/System/AssetManager.cpp
        src/System/SystemManager.cpp
        src/Engine/Engine.cpp
)

target_precompile_headers(WeevilEngine
        PRIVATE
        include/WeevilEngine/weevil.h
)

target_include_directories(WeevilEngine
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        src/include
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

add_executable(WeevilApp src/main.cpp)
target_link_libraries(WeevilApp PRIVATE WeevilEngine)

# Add the game module target.
add_library(GameModule SHARED
        Game/Game.cpp
)
target_compile_features(GameModule PUBLIC cxx_std_20)
target_compile_options(GameModule PRIVATE -Wall -Wextra -Wpedantic -Wno-unknown-pragmas)

target_include_directories(GameModule PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(GameModule
        PRIVATE
        SDL3_image::SDL3_image
        SDL3::SDL3
        EnTT::EnTT
        spdlog::spdlog
        tomlplusplus::tomlplusplus
)

# Place the resulting shared library in a “modules” folder (which your engine expects).
set_target_properties(GameModule PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules
)
