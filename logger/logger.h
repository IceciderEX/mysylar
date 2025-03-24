#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <string>
#include "logevent.h"

class Logger {
public:
    Logger(const std::string& name);
    ~Logger();

    void add_appender(LogAppender::ptr appender);
    void delete_Appender(LogAppender::ptr appender);

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level level) { m_level = level; }

    const std::string& getName() const { return m_name; }
private:
    std::string m_name;
    LogLevel::Level m_level;
};


#endif