make_executable()

target_link_libraries(${PROJECT}
    EnTT::EnTT
    imgui
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
