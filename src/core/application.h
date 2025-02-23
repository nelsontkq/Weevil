#pragma once
#include <weevil/pch.h>

#include "core/module_manager.h"
#include "platform/common/platform.h"

namespace wv {
class Application {
 public:
  SDL_AppResult init();
  SDL_AppResult process_event(SDL_Event& event);
  SDL_AppResult iterate();
  void shutdown();

 private:
  bool quitting_ = false;
  wv::Platform platform_;
  uint64_t delta_ticks_ = 0;
  ModuleManager module_manager_;
  wv::AppSettings settings_;
};
}  // namespace wv