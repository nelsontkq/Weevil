include(FetchContent)

set(OpenGL_GL_PREFERENCE GLVND)
# OpenGL (optional, as it's usually provided by the system)
# You can keep using find_package if preferred

# SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.28.2  # Replace with the desired version
)
FetchContent_MakeAvailable(SDL2)

# ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.9  # Replace with the desired version
)
FetchContent_MakeAvailable(imgui)

# EnTT
FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG v3.11.1  # Replace with the desired version
)
FetchContent_MakeAvailable(EnTT)

# GLAD (optional)
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36  # Replace with the desired version
)
FetchContent_MakeAvailable(glad)
