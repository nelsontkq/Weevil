#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#include <memory>

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace wv {

class Log {
 public:
  static void Init();

  inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() {
    return s_CoreLogger;
  }

  inline static std::shared_ptr<spdlog::logger> &GetClientLogger() {
    return s_ClientLogger;
  }

 private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;

};
// #ifdef WV_LOG_NONE
// // Core log macros
// #define CORE_TRACE(...) ((void)0)
// #define CORE_INFO(...) ((void)0)
// #define CORE_WARN(...) ((void)0)
// #define CORE_ERROR(...) ((void)0)
// #define CORE_CRITICAL(...) ((void)0)

// // Client log macros
// #define LOG_TRACE(...) ((void)0)
// #define LOG_INFO(...) ((void)0)
// #define LOG_WARN(...) ((void)0)
// #define LOG_ERROR(...) ((void)0)
// #define LOG_CRITICAL(...) ((void)0)

// #else
// Core log macros
#define CORE_TRACE(...) ::wv::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::wv::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::wv::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::wv::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::wv::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...) ::wv::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::wv::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::wv::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::wv::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::wv::Log::GetClientLogger()->critical(__VA_ARGS__)
// #endif
}  // namespace wv

#pragma clang diagnostic pop