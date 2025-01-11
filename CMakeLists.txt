cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)
add_subdirectory(dependencies)

# Enable clang-tidy analysis
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
    # Set clang-tidy to use the most opinionated level
    set(CLANG_TIDY_CHECKS "*,clang-analyzer-*,modernize-*,performance-*,readability-*,portability-*")
    set(CLANG_TIDY_WARNINGS_AS_ERRORS "*,clang-analyzer-*")

    # Configure clang-tidy as a compiler launcher
    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE};
        -checks=${CLANG_TIDY_CHECKS};
        -warnings-as-errors=${CLANG_TIDY_WARNINGS_AS_ERRORS};
        --extra-arg=-std=c++20")
endif()

add_executable(main src/main.cpp
        src/Game/Game.cpp
        src/Game/Game.hpp
        src/States/MenuState.cpp
        src/States/MenuState.hpp
        src/Entities/Entity.hpp
        src/Resources/Assets.cpp
        src/Resources/Assets.hpp
        src/Resources/Logger.cpp
        src/Resources/Logger.hpp
        src/States/MainGameState.cpp
        src/States/MainGameState.hpp
)

# Add new ECS files
# Components
src/Components/PositionComponent.hpp
src/Components/RenderComponent.hpp
# Systems
src/Systems/RenderSystem.cpp
src/Systems/RenderSystem.hpp

# Add EnTT as a dependency
add_subdirectory(dependencies/entt)

# Include directories for EnTT
target_include_directories(main PRIVATE dependencies/entt/src)

# Link EnTT to your executable
target_link_libraries(main PRIVATE EnTT)


target_link_libraries(main PRIVATE ImGui-SFML::ImGui-SFML SFML::Audio SFML::Graphics)
