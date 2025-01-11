cmake_minimum_required(VERSION 3.28)
project(KingsCourt LANGUAGES CXX)
# 3rd party libraries
include(cmake/3rdparty.cmake)
# macro and configurations
include(cmake/config.cmake)

# Enable clang-tidy analysis
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

add_subdirectory(src)