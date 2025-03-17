#ifndef __LOG_EVENT_H__
#define __LOG_EVENT_H__

#include <string>
#include "define.h"
#include "logger.h"

class LogEvent {
public:
    LogEvent();
    ~LogEvent();
    std::string getLogContent() const;
    std::string getLoggerName() const;
    LOG_LEVEL getLogLevel() const;
    std::string getFileName() const;
    uint32_t getLineNumber() const;
    uint32_t getProgramRunnedTime() const;
    uint32_t getThreadId() const;
    uint32_t getCoroutineId() const;
    std::string getThreadName() const;
    uint64_t getTimeStamp() const;
private:
    std::string m_log_content; // 日志内容
    std::string m_logger_name; // 日志器名
    LOG_LEVEL m_log_level; // 日志级别
    std::string m_file_name; // 文件名
    uint32_t m_line_number; // 行号
    uint32_t m_program_runned_time; // 运行时间, sylar::getElapsedMS()
    uint32_t m_thread_id; // 线程id
    uint32_t m_coroutine_id; // 协程id
    std::string m_thread_name; // 线程名
    uint64_t m_time_stamp; // 时间戳
};

#endif