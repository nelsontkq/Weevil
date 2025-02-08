
#include <weevil/core/log.h>

#include <spdlog/sinks/stdout_color_sinks.h>

namespace wv {
#ifdef WV_LOG_NONE
void Log::Init() {}
#else
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
  // Set pattern: [%Time%] %LoggerName%: %Message%
  spdlog::set_pattern("%^[%T] %n: %v%$");

  // Initialize Core Logger
  s_CoreLogger = spdlog::stdout_color_mt("CORE");

  // Initialize Client Logger
  s_ClientLogger = spdlog::stdout_color_mt("APP");
#ifdef WV_LOG_TRACE
  s_CoreLogger->set_level(spdlog::level::trace);
  s_ClientLogger->set_level(spdlog::level::trace);
#elif defined(WV_LOG_INFO)
  s_CoreLogger->set_level(spdlog::level::info);
  s_ClientLogger->set_level(spdlog::level::info);
#elif defined(WV_LOG_WARN)
  s_CoreLogger->set_level(spdlog::level::warn);
  s_ClientLogger->set_level(spdlog::level::warn);
#elif defined(WV_LOG_ERROR)
  s_CoreLogger->set_level(spdlog::level::err);
  s_ClientLogger->set_level(spdlog::level::err);
#endif
}
#endif
}  // namespace wv