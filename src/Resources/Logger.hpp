#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <vector>
#include <string>
#include <mutex>

class Logger {
public:
    static Logger& getInstance();

    void log(const std::string& message);

    void clear();

    const std::vector<std::string>& getMessages() const;

private:
    Logger() = default;
    std::vector<std::string> m_messages;
    mutable std::mutex m_mutex;
};

#endif // LOGGER_HPP
