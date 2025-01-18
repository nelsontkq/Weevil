cmake_minimum_required(VERSION 3.0)

project(imgui)

# Define the source files
set(IMGUI_SRC
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_tables.cpp
    imgui_demo.cpp
    backends/imgui_impl_sdl2.cpp
    backends/imgui_impl_sdlrenderer2.cpp
)

# Add the library target
add_library(imgui ${IMGUI_SRC})

# Include directories
target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# Find SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(imgui PUBLIC SDL2::SDL2 SDL2::SDL2main)
