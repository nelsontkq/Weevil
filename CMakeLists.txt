cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable(main src/main.cpp
        src/Game/Game.cpp
        src/Game/Game.h
        src/Resources/AssetLoader.cpp
        src/Resources/AssetLoader.h
        src/States/GameState.hpp
        src/States/MenuState.cpp
        src/States/MenuState.h
        src/Entities/Entity.h
)
target_link_libraries(main PRIVATE SFML::Graphics)
