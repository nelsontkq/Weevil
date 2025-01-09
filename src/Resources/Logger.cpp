#include "Logger.hpp"

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_messages.push_back(message);
}

void Logger::clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_messages.clear();
}

const std::vector<std::string>& Logger::getMessages() const {
    return m_messages;
}
