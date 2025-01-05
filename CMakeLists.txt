cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
include(ExternalProject)


# Add TGUI include and library paths
set(TGUI_INCLUDE_DIR ${CMAKE_BINARY_DIR}/TGUI/include)
set(TGUI_LIBRARY_DIR ${CMAKE_BINARY_DIR}/TGUI/lib)

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

add_dependencies(main TGUI)

target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:main>/assets
)
