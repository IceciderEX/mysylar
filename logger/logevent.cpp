#include "logevent.h"

std::string LogEvent::getLogContent() const {
    return m_log_content;
}

std::string LogEvent::getLoggerName() const {
    return m_logger_name;
}

LOG_LEVEL LogEvent::getLogLevel() const {
    return m_log_level;
}

std::string LogEvent::getFileName() const {
    return m_file_name;
}

uint32_t LogEvent::getLineNumber() const {
    return m_line_number;
}

uint32_t LogEvent::getProgramRunnedTime() const {
    return m_program_runned_time;
}

uint32_t LogEvent::getThreadId() const {
    return m_thread_id;
}

uint32_t LogEvent::getCoroutineId() const {
    return m_coroutine_id;
}

std::string LogEvent::getThreadName() const {
    return m_thread_name;
}

