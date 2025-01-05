cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
find_package(TGUI REQUIRED)

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
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio TGUI)

add_custom_command(TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:main>/assets
)
