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
add_executable(Weevil
        src/System/SystemManager.cpp
        src/System/SystemManager.h
        src/System/System.h
        src/System/SystemManager.h
        src/System/SystemManager.h
        src/System/System.h)

# Specify source files
target_sources(Weevil PRIVATE
    src/main.cpp
    src/Game/Game.cpp
    src/Utils/UUID.cpp
    src/Core/Log.cpp
    src/Core/AppSettings.cpp
    src/Core/SDLContext.cpp
    src/System/Scheduler.cpp
    src/System/Scheduler.h
)

# Include directories
target_include_directories(Weevil PRIVATE src ${3RD_PARTY_INCLUDE_DIRS})

# Link libraries
target_link_libraries(Weevil PRIVATE ${3RD_PARTY_LIBS})

# Enable precompiled headers
target_precompile_headers(Weevil PRIVATE src/wvpch.h)

# Ensure that the compiler supports C++20 features
target_compile_features(Weevil PRIVATE cxx_std_20)
