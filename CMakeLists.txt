cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(dependencies)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Add the executable and include the necessary source files
add_executable(main 
    src/main.cpp 
    src/Game/Game.cpp 
    src/Game/Game.hpp 
    src/Utils/SDL_Deleter.hpp
)
target_compile_features(main PRIVATE cxx_std_20)

target_include_directories(main PRIVATE ${3RD_PARTY_INCLUDE_DIRS})

# Link libs
target_link_libraries(main PRIVATE ${3RD_PARTY_LIBS})
