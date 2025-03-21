#include <iostream>
#include "logappender.h"
#include "logformatter.h"

LogAppender::LogAppender(LogFormatter::ptr default_formatter): m_formatter(default_formatter) {
    return;
}

void LogAppender::setFormatter(LogFormatter::ptr formatter) {
    m_formatter = formatter;
    return;
}

LogFormatter::ptr LogAppender::getFormatter() {
    return m_formatter;
}

StdoutLogAppender::StdoutLogAppender(): LogAppender(LogFormatter::ptr(new LogFormatter())) {
}

void StdoutLogAppender::log(LogEvent::ptr event) {
    if(m_formatter) {
        m_formatter->format(std::cout, event);
    } else {
        m_formatter->format(std::cout, event);
    }
    return;
}

FileLogAppender::FileLogAppender(): LogAppender(LogFormatter::ptr(new LogFormatter())) {
    return;
}
