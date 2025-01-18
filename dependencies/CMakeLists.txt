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

# EnTT
set(ENTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/entt/src")

# ImGui
add_subdirectory(imgui)

# Link SDL2 libraries to imgui
target_link_libraries(imgui PUBLIC
    SDL2::SDL2
    SDL2::SDL2main
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
