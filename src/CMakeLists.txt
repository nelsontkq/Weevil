include(${CMAKE_SOURCE_DIR}/cmake/3rdparty.cmake)

include_directories(
    ${IMGUI_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${glad_INCLUDE_DIRS}
    ${EnTT_INCLUDE_DIRS}
)

add_all_subdirectories()

