cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
include(ExternalProject)

# Fetch TGUI library
ExternalProject_Add(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG v1.7.0  # Use a specific version or branch
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/tgui
)

# Add TGUI include and library paths
set(TGUI_INCLUDE_DIR ${CMAKE_BINARY_DIR}/tgui/include)
set(TGUI_LIBRARY_DIR ${CMAKE_BINARY_DIR}/tgui/lib)

include_directories(${TGUI_INCLUDE_DIR})
link_directories(${TGUI_LIBRARY_DIR})

add_executable(main src/main.cpp
        src/Game/Game.cpp
        src/Game/Game.hpp
        src/States/MenuState.cpp
        src/States/MenuState.hpp
        src/Entities/Entity.hpp
        src/Resources/Assets.cpp
        src/Resources/Assets.hpp
        src/States/MainGameState.cpp
        src/States/MainGameState.hpp
)

# Ensure TGUI is built before the main target
add_dependencies(main TGUI)

target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio tgui)

add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:main>/assets
)
