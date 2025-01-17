
#include "Log.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace wv {
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
  // Set pattern: [%Time%] %LoggerName%: %Message%
  spdlog::set_pattern("%^[%T] %n: %v%$");

  // Initialize Core Logger
  s_CoreLogger = spdlog::stdout_color_mt("CORE");
  s_CoreLogger->set_level(spdlog::level::trace);

  // Initialize Client Logger
  s_ClientLogger = spdlog::stdout_color_mt("APP");
  s_ClientLogger->set_level(spdlog::level::trace);
}

}