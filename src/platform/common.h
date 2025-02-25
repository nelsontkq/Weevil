#pragma once
#include "common/platform.h"
#ifdef WV_PLATFORM_LINUX
#include "linux/hot_reloader.h"
#else
#error hot reloading not supported on this platform!
#endif
