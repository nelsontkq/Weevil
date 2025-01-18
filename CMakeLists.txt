cmake_minimum_required(VERSION 3.27)
project(Weevil LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(dependencies)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add the executable
# Create a library target for your engine
add_library(WeevilEngine STATIC
    src/System/SystemManager.cpp
    src/Utils/UUID.cpp
    src/Core/Log.cpp
    src/Core/AppSettings.cpp
    src/Core/SDLContext.cpp
    src/Game/Game.cpp
    # Add other engine source files as needed
)

# Create an executable target for testing
add_executable(WeevilTest
    src/main.cpp
)

# Link the test executable against your engine library
target_link_libraries(WeevilTest PRIVATE WeevilEngine)

# Include directories for the engine library
target_include_directories(WeevilEngine PUBLIC include)

# Include directories for the test executable
target_include_directories(WeevilTest PRIVATE ${3RD_PARTY_INCLUDE_DIRS})

# Link third-party libraries to the engine library
target_link_libraries(WeevilEngine PUBLIC ${3RD_PARTY_LIBS})

# Set C++ standards and features for the engine library
target_compile_features(WeevilEngine PUBLIC cxx_std_20)

# Set C++ standards and features for the test executable
target_compile_features(WeevilTest PRIVATE cxx_std_20)

# Enable precompiled headers for the engine library
target_precompile_headers(WeevilEngine PRIVATE src/wvpch.h)

# Enable precompiled headers for the test executable
target_precompile_headers(WeevilTest PRIVATE src/wvpch.h)

# Installation rules (optional)
install(TARGETS WeevilEngine spdlog imgui
    EXPORT WeevilEngineTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY src/
    DESTINATION include/WeevilEngine
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# Remove or comment out the export command
# export(TARGETS WeevilEngine FILE WeevilEngineTargets.cmake)

# Use install(EXPORT ...) to export the targets during installation
install(EXPORT WeevilEngineTargets
    FILE WeevilEngineTargets.cmake
    NAMESPACE WeevilEngine::
    DESTINATION lib/cmake/WeevilEngine
)
