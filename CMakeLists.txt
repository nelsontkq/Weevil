cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)
add_subdirectory(dependencies)

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
        src/Utils/Logger.cpp
        src/Utils/Logger.hpp
)


target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML SFML::Audio SFML::Graphics)
