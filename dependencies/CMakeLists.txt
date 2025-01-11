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
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.6.tar.gz
  URL_MD5 9df042eef9394fb91251a7c196eaa13b
)

FetchContent_MakeAvailable(imgui)