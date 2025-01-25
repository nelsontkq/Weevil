#pragma once

#include "WeevilEngine/wvpch.h"
struct SDL_Deleter {
  void operator()(SDL_Window *ptr) const {
    if (ptr) {
      SDL_DestroyWindow(ptr);
      LOG_TRACE("Window destroyed");
    }
  }

  void operator()(SDL_Renderer *ptr) const {
    if (ptr) {
      SDL_DestroyRenderer(ptr);
      LOG_TRACE("Renderer destroyed");
    }
  }
  void operator()(SDL_GPUDevice *ptr) const {
    if (ptr) {
      SDL_DestroyGPUDevice(ptr);
      LOG_TRACE("GPU Device destroyed");
    }
  }

  void operator()(SDL_Texture *ptr) const {
    if (ptr) {
      SDL_DestroyTexture(ptr);
    }
  }
};
