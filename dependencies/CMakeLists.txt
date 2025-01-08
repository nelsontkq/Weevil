include(FetchContent)

# SFML
FetchContent_Declare(
  sfml
  URL https://github.com/SFML/SFML/archive/refs/tags/3.0.0.tar.gz
  URL_MD5 631426f02fb46ccefc896975f9c44c7f
)
FetchContent_MakeAvailable(sfml)

# Dear ImGui
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.6.tar.gz
  URL_MD5 9df042eef9394fb91251a7c196eaa13b
)

FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
  imgui-sfml
  URL https://github.com/SFML/imgui-sfml/archive/refs/tags/v3.0.tar.gz
  URL_MD5 94ca2010de8745cd2d642d1da1929b17
)
FetchContent_MakeAvailable(imgui-sfml)
