include(FetchContent)

# EnTT
FetchContent_Declare(
  entt
  URL https://github.com/skypjack/entt/archive/refs/tags/v3.14.0.tar.gz
  URL_MD5 dfb9f81fae974fa7cfcca12d1f690c49
)
FetchContent_MakeAvailable(entt)