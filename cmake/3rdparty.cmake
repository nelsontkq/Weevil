include(FetchContent)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  find_package(X11 REQUIRED)

  if (NOT X11_Xi_FOUND)
    message(FATAL_ERROR "X11 Xi library is required")
  endif ()
endif ()

# ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.6
)
FetchContent_MakeAvailable(imgui)

# EnTT
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.14.0
)
FetchContent_MakeAvailable(EnTT)
set(ENTT_LIBRARIES EnTT::EnTT)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
)
FetchContent_MakeAvailable(glad)

set(IMGUI_INCLUDE_DIR ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
set(IMGUI_LIBRARIES imgui)
file(GLOB IMGUI_SOURCES ${IMGUI_INCLUDE_DIR}/*.cpp)

set(IMGUI_INCLUDE_DIR ${IMGUI_INCLUDE_DIR} PARENT_SCOPE)
set(IMGUI_LIBRARIES ${IMGUI_LIBRARIES} PARENT_SCOPE)
set(ENTT_LIBRARIES ${ENTT_LIBRARIES} PARENT_SCOPE)
file(GLOB IMGUI_HEADERS ${IMGUI_INCLUDE_DIR}/*.h)
                 
add_library(imgui STATIC ${IMGUI_HEADERS} ${IMGUI_SOURCES})

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
    
target_link_libraries(imgui
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES})
    
set_target_properties(imgui PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(imgui PROPERTIES FOLDER 3rdparty)
