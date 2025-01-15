#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

// Log is a singleton class that wraps spdlog logger
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
#define CORE_TRACE(...)    ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...)     ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)      ::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)      ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)     ::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)  ::Log::GetClientLogger()->critical(__VA_ARGS__)
