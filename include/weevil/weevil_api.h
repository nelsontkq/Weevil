#pragma once

#include <weevil/core/components.h>
#include <weevil/core/dispatchables.h>
#include <weevil/core/imodule.h>
#include <weevil/core/log.h>
#include <weevil/core/rngen.h>
#include <weevil/core/uuid.h>
#include <weevil/core/window.h>

#include <entt/entt.hpp>

#define WV_MODULE(module)                                  \
  extern "C" {                                             \
  wv::IModule* wv_create_module() { return new module(); } \
  }
