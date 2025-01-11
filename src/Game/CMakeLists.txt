make_executable()

# Set the ImGui source directory
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# Include directories
target_include_directories(${PROJECT} PRIVATE
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${SDL2_SOURCE_DIR}/include
  ${OPENGL_INCLUDE_DIR}
)

# Add ImGui source files
file(GLOB IMGUI_SOURCES
  ${IMGUI_DIR}/*.cpp
  ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)
target_sources(${PROJECT} PRIVATE ${IMGUI_SOURCES})

# Define IMGUI_IMPL_OPENGL_LOADER_GLAD
target_compile_definitions(${PROJECT} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# Include glad sources if needed
target_include_directories(${PROJECT} PRIVATE ${glad_SOURCE_DIR}/include)
file(GLOB GLAD_SOURCES ${glad_SOURCE_DIR}/src/*.c)
target_sources(${PROJECT} PRIVATE ${GLAD_SOURCES})

# Link libraries
target_link_libraries(${PROJECT}
  EnTT::EnTT
  SDL2::SDL2
  ${OPENGL_LIBRARIES}
)
