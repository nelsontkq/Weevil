make_executable()


include_directories(
    ${IMGUI_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
    ${glad_INCLUDE_DIRS}       # Add this line if glad include dirs are needed
    ${EnTT_INCLUDE_DIRS}       # Add this line if EnTT include dirs are needed
)

target_link_libraries(${PROJECT}
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${IMGUI_LIBRARIES}
    ${ENTT_LIBRARIES}
    glad
    )
