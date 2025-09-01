#include "logger.h"

void Logger::add_appender(LogAppender::ptr appender) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(appender->getFormatter() == nullptr) {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delete_appender(LogAppender::ptr appender) {
    std::lock_guard<std::mutex> lock(m_mutex);
    for(auto it = m_appenders.begin(); it != m_appenders.end(); it++) {
        if(*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
    return;
}

void Logger::clear_appenders() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_appenders.clear();
}


void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if(level < m_level) {
        return;
    }
    auto self = shared_from_this();
    std::lock_guard<std::mutex> lock(m_mutex);
    for(auto& appender : m_appenders) {
        appender->log(self, level, event);
    }
}