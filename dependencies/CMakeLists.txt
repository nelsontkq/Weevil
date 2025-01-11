include(FetchContent)

# EnTT
FetchContent_Declare(
  entt
  URL https://github.com/skypjack/entt/archive/refs/tags/v3.14.0.tar.gz
  URL_MD5 dfb9f81fae974fa7cfcca12d1f690c49
)
FetchContent_MakeAvailable(entt)


# Dear ImGui
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.89.7.tar.gz
)

FetchContent_MakeAvailable(imgui)

# Add ImGui library target
add_library(imgui_library
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp    # Include demo for testing
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer.cpp
)

target_include_directories(imgui_library PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui_library PUBLIC SDL2::SDL2)
