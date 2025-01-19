cmake_minimum_required(VERSION 3.27)
project(Weevil LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-dev")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
# CPM
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.3/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=cc155ce02e7945e7b8967ddfaff0b050e958a723ef7aad3766d368940cb15494
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
## Dependencies
set(LIBS)
CPMAddPackage("gh:libsdl-org/SDL#prerelease-3.1.10")
list(APPEND LIBS SDL3::SDL3)
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE) # Requires Perl and Nasm for libaom (AVIF image format)
CPMAddPackage("gh:libsdl-org/SDL_image#1ca9bd8")
list(APPEND LIBS SDL3_image::SDL3_image)
# glm
CPMAddPackage("gh:g-truc/glm#3c18b0f")
list(APPEND LIBS glm::glm-header-only)
# assimp
CPMAddPackage("gh:assimp/assimp#5fa7b8c")
list(APPEND LIBS assimp::assimp)


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
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/vendored/imgui
        ${CMAKE_SOURCE_DIR}/vendored/imgui/backends
        ${CMAKE_SOURCE_DIR}/vendored/entt/single_include
        ${CMAKE_SOURCE_DIR}/vendored/spdlog/include
)

target_compile_features(WeevilEngine PRIVATE cxx_std_23)
target_link_libraries(WeevilEngine
        PUBLIC
        ${LIBS}
)

# Create an executable target for testing
add_executable(WeevilTest
        src/main.cpp
)
# Link WeevilEngine to the test executable
target_link_libraries(WeevilTest
        PRIVATE
        WeevilEngine
)
