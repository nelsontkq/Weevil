#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// Log is a singleton class that wraps spdlog logger
namespace wv {

class Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define CORE_TRACE(...)    ::wv::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::wv::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::wv::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::wv::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::wv::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...)     ::wv::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)      ::wv::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)      ::wv::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     ::wv::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)  ::wv::Log::GetClientLogger()->critical(__VA_ARGS__)

} // namespace wv

#endif // WV_LOG_HPP
