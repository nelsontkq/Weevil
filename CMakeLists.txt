cmake_minimum_required(VERSION 3.27)
project(Weevil LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_subdirectory(dependencies)
find_package(SDL2 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# ImGui source files (assuming imgui is in dependencies/imgui)
set(IMGUI_SOURCES
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_demo.cpp
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_sdl2.cpp
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/backends/imgui_impl_sdlrenderer2.cpp
)

# Add the executable
# Create a library target for your engine
add_library(WeevilEngine STATIC
        ${IMGUI_SOURCES}
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
        ${CMAKE_SOURCE_DIR}/dependencies/imgui
        ${CMAKE_SOURCE_DIR}/dependencies/imgui/backends
        ${CMAKE_SOURCE_DIR}/dependencies/entt/single_include
        ${CMAKE_SOURCE_DIR}/dependencies/spdlog/include
)

target_link_libraries(WeevilEngine
        PUBLIC
        SDL2::SDL2
        nlohmann_json::nlohmann_json
        spdlog::spdlog
        EnTT::EnTT
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
