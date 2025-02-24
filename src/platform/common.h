#pragma once
#include "common/rendering/renderer.h"
#ifdef WV_PLATFORM_LINUX
#include "linux/hot_reloader.h"
#else
#error hot reloading not supported on this platform!
#endif
