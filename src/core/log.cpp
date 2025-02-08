
#include "WeevilEngine/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace wv {
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
  // Set pattern: [%Time%] %LoggerName%: %Message%
  spdlog::set_pattern("%^[%T] %n: %v%$");

  // Initialize Core Logger
  s_CoreLogger = spdlog::stdout_color_mt("CORE");

  // Initialize Client Logger
  s_ClientLogger = spdlog::stdout_color_mt("APP");
#if WV_LOG_LEVEL == 4
  s_CoreLogger->set_level(spdlog::level::trace);
  s_ClientLogger->set_level(spdlog::level::trace);
#elif WV_LOG_LEVEL == 3
  s_CoreLogger->set_level(spdlog::level::info);
  s_ClientLogger->set_level(spdlog::level::info);
#elif WV_LOG_LEVEL == 2
  s_CoreLogger->set_level(spdlog::level::warn);
  s_ClientLogger->set_level(spdlog::level::warn);
#elif WV_LOG_LEVEL == 1
  s_CoreLogger->set_level(spdlog::level::err);
  s_ClientLogger->set_level(spdlog::level::err);
#endif
}

}  // namespace wv