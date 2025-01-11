include(FetchContent)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  find_package(X11 REQUIRED)

  if (NOT X11_Xi_FOUND)
    message(FATAL_ERROR "X11 Xi library is required")
  endif ()
endif ()

# Enable clang-tidy analysis
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

# EnTT
FetchContent_Declare(
  entt
  URL https://github.com/skypjack/entt/archive/refs/tags/v3.14.0.tar.gz
  URL_MD5 dfb9f81fae974fa7cfcca12d1f690c49
)
FetchContent_MakeAvailable(entt)
list(APPEND 3RD_PARTY_INCLUDE_DIRS ${entt_SOURCE_DIR}/src)


# Dear ImGui
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.6.tar.gz
  URL_MD5 9df042eef9394fb91251a7c196eaa13b
)

FetchContent_MakeAvailable(imgui)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(3RD_PARTY_LIBS EnTT::EnTT ${SDL2_LIBRARIES} imgui)
set(3RD_PARTY_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${imgui_SOURCE_DIR})
# print 3RD_PARTY_LIBS
message(STATUS "3RD_PARTY_LIBS: ${3RD_PARTY_LIBS}")
