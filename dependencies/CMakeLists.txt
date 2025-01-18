cmake_minimum_required(VERSION 3.5)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  find_package(X11 REQUIRED)

  if (NOT X11_Xi_FOUND)
    message(FATAL_ERROR "X11 Xi library is required")
  endif ()
endif ()

# spdlog
add_subdirectory(spdlog)

# EnTT (header-only library)
add_library(entt INTERFACE)
target_include_directories(entt INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/entt/src>
    $<INSTALL_INTERFACE:include>
)

# Install EnTT headers
install(DIRECTORY entt/src/entt
    DESTINATION include/entt
)

# Install and export entt
install(TARGETS entt
    EXPORT enttTargets
)

install(EXPORT enttTargets
    FILE enttTargets.cmake
    NAMESPACE entt::
    DESTINATION lib/cmake/entt
)

# ImGui
add_subdirectory(imgui)

# Link SDL2 libraries to imgui
target_link_libraries(imgui PUBLIC
    SDL2::SDL2
    SDL2::SDL2main
)

# Installation Rules for imgui
install(TARGETS imgui
    EXPORT imguiTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include/imgui
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/imgui/
    DESTINATION include/imgui
    FILES_MATCHING PATTERN "*.h"
    PATTERN "backends" EXCLUDE
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/
    DESTINATION include/imgui/backends
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT imguiTargets
    FILE imguiTargets.cmake
    NAMESPACE imgui::
    DESTINATION lib/cmake/imgui
)

# Collect third-party include directories
set(3RD_PARTY_INCLUDE_DIRS
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
    ${ENTT_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
    ${nlohmann_json_INCLUDE_DIRS}
    PARENT_SCOPE
)

# Collect third-party libraries
set(3RD_PARTY_LIBS
    spdlog::spdlog
    imgui
    SDL2::SDL2
    SDL2::SDL2main
    nlohmann_json::nlohmann_json
    PARENT_SCOPE
)
install(FILES
    ${imgui_external_SOURCE_DIR}/imgui.h
    ${imgui_external_SOURCE_DIR}/imconfig.h
    DESTINATION include/imgui
)

install(DIRECTORY
    ${imgui_external_SOURCE_DIR}/backends
    DESTINATION include/imgui/backends
    FILES_MATCHING PATTERN "*.h"
)

install(TARGETS imgui
    EXPORT imguiTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
