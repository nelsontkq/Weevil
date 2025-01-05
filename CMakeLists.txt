cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)
set(TGUI_BACKEND SFML_GRAPHICS)
include(ExternalProject)

# Fetch SFML library
ExternalProject_Add(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/SFML
)

# Fetch TGUI library
ExternalProject_Add(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG v1.7.0  # Use a specific version or branch
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/TGUI
        -DSFML_DIR=${CMAKE_BINARY_DIR}/SFML/lib/cmake/SFML  # Set SFML_DIR for TGUI
)

# Add SFML include and library paths
set(SFML_INCLUDE_DIR ${CMAKE_BINARY_DIR}/SFML/include)
set(SFML_LIBRARY_DIR ${CMAKE_BINARY_DIR}/SFML/lib)

# Add TGUI include and library paths
set(TGUI_INCLUDE_DIR ${CMAKE_BINARY_DIR}/TGUI/include)
set(TGUI_LIBRARY_DIR ${CMAKE_BINARY_DIR}/TGUI/lib)

include_directories(${SFML_INCLUDE_DIR} ${TGUI_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR} ${TGUI_LIBRARY_DIR})

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

# Ensure SFML and TGUI are built before the main target
add_dependencies(main SFML TGUI)

target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio TGUI)

add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:main>/assets
)
