#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <string>
#include <vector>
#include "logevent.h"
#include "logappender.h"

class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    
    Logger(const std::string& name);
    ~Logger();

    void add_appender(LogAppender::ptr appender);
    void delete_appender(LogAppender::ptr appender);
    void clear_appenders();

    void log(LogLevel::Level level, const LogEvent::ptr event);
    void debug(const LogEvent::ptr event);
    void info(const LogEvent::ptr event);
    void warn(const LogEvent::ptr event);
    void error(const LogEvent::ptr event);
    void fatal(const LogEvent::ptr event);

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level level) { m_level = level; }

    const std::string& getName() const { return m_name; }
private:
    std::string m_name;
    LogLevel::Level m_level;
    std::vector<LogAppender::ptr> m_appenders;
    std::mutex m_mutex;
    // default formatter
    LogFormatter::ptr m_formatter;
};


#endif