#ifndef __LOG_APPENDER_H__
#define __LOG_APPENDER_H__

#include <memory>
#include <mutex>
#include <fstream>
#include "logformatter.h"
#include "logevent.h"

/*
LogAppender是一个抽象类，定义了日志输出的接口，所有的日志输出类都需要继承LogAppender类，并实现其接口。
*/
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    LogAppender(LogFormatter::ptr formatter); // default_formatter
    virtual ~LogAppender();

    void setFormatter(LogFormatter::ptr formatter);
    LogFormatter::ptr getFormatter();

    virtual void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) = 0;
protected:
    // m_mutex is used to ensure thread safety when accessing.(multiple log events)
    std::mutex m_mutex;
    LogFormatter::ptr m_formatter; // 日志格式化器（用于将输入的event进行格式化）
    LogFormatter::ptr m_default_formatter; // 默认日志格式化器
};


class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    StdoutLogAppender();
    ~StdoutLogAppender();

    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
private:
    
};


class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr; 

    FileLogAppender(const std::string& file_name);
    ~FileLogAppender();

    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    bool reopen_file();
private:
    std::string m_filename;
    std::ofstream m_filestream; // 文件流，用于传入给LogFormatter写入日志
    bool m_reopen_error; // 重新打开文件 bool
    uint64_t m_last_reopen_time; // 上次重新打开文件的时间
    LogLevel::Level m_level; // 该appender的日志等级
};


# endif