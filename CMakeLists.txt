cmake_minimum_required(VERSION 3.27)
project(Weevil LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.5/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=c46b876ae3b9f994b4f05a4c15553e0485636862064f1fcc9d8b4f832086bc5d
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
## Dependencies
set(LIBS)
CPMAddPackage("gh:libsdl-org/SDL#prerelease-3.1.10")
CPMAddPackage("gh:libsdl-org/SDL_image#preview-3.1.0")
CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:skypjack/entt@3.14.0")
CPMAddPackage("gh:gabime/spdlog@1.15.0")

add_library(WeevilEngine STATIC
        src/Utils/UUID.cpp
        src/Core/Log.cpp
        src/Core/AppSettings.cpp
        src/Core/SDLContext.cpp
        src/Game/Game.cpp
        src/System/SystemManager.cpp
)


# Set include directories for WeevilEngine
target_include_directories(WeevilEngine
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include

)

target_compile_features(WeevilEngine PRIVATE cxx_std_23)
target_link_libraries(WeevilEngine
        PUBLIC
        SDL3::SDL3
        SDL3_image::SDL3_image
        EnTT::EnTT
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)

# Create an executable target for testing
add_executable(WeevilTest
        src/main.cpp
)
target_compile_features(WeevilTest PRIVATE cxx_std_23)
# Link WeevilEngine to the test executable
target_link_libraries(WeevilTest
        PRIVATE
        WeevilEngine
)
