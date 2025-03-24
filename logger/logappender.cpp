#include <iostream>
#include <string>
#include <mutex>
#include "logappender.h"
#include "logformatter.h"

LogAppender::LogAppender(LogFormatter::ptr default_formatter): m_formatter(default_formatter) {
    return;
}

void LogAppender::setFormatter(LogFormatter::ptr formatter) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_formatter = formatter;
    return;
}

LogFormatter::ptr LogAppender::getFormatter() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_formatter == nullptr) {
        return m_default_formatter;
    }
    return m_formatter;
}

StdoutLogAppender::StdoutLogAppender(): LogAppender(LogFormatter::ptr(new LogFormatter())) {
    return;
}

// 将日志event输出到标准输出
void StdoutLogAppender::log(LogEvent::ptr event) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_formatter) {
        m_formatter->format(std::cout, event);
    } else {
        m_default_formatter->format(std::cout, event);
    }
    return;
}

/****************** FileLogAppender ********************/
// 构造时，将file_name对应的文件打开，如果打开失败，输出错误信息
FileLogAppender::FileLogAppender(const std::string& file_name)
    : LogAppender(LogFormatter::ptr(new LogFormatter())) {
    m_filename = file_name;
    auto open_result = reopen_file();
    if(!open_result) {
        std::cout << "open file " << m_filename << " failed" << std::endl;
    }
    return;
}

// reopen m_filename corresponding file
// if m_filestream is open, return true
bool FileLogAppender::reopen_file() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_filestream.is_open()) {
        m_filestream.close();
    }
    m_filestream.open(m_filename, std::ios::app);
    return m_filestream.is_open();
}

void FileLogAppender::log(LogEvent::ptr event) {
    std::lock_guard<std::mutex> lock(m_mutex);
    uint64_t now = event->getTimeStamp();
    // 如果一个日志事件距离上次重新打开文件的时间超过60s，则重新打开文件
    if(now - m_last_reopen_time > 60) {
        if(!reopen_file()) {
            m_reopen_error = true;
            return;
        }
        m_last_reopen_time = now;
    }
    if(m_formatter) {
        m_formatter->format(m_filestream, event);
    } else {
        m_default_formatter->format(m_filestream, event);
    }
    return;
}
