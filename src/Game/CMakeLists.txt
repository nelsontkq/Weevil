
target_compile_features(main PRIVATE cxx_std_20)

make_executable()

target_link_libraries(${PROJECT}
    EnTT::EnTT
    imgui
    SDL2-static  # Use SDL2-static if SDL2 provides a static library target
    glad  # If you're using GLAD
    ${OPENGL_LIBRARIES}  # If you're still using system OpenGL
)
