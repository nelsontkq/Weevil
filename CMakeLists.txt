cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)
add_subdirectory(dependencies)

# Enable clang-tidy analysis
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()
# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(include)


# Add the executable and include the necessary source files
add_executable(main src/main.cpp src/Game/Game.cpp)

target_compile_features(main PRIVATE cxx_std_20)
# Link libs
target_link_libraries(main PRIVATE EnTT::EnTT ${SDL2_LIBRARIES} imgui)
