#ifndef __LOG_APPENDER_H__
#define __LOG_APPENDER_H__

#include <memory>
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

    virtual void log(LogEvent::ptr event) = 0;
protected:
    LogFormatter::ptr m_formatter;
    LogFormatter::ptr m_default_formatter;
};


class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    StdoutLogAppender();
    ~StdoutLogAppender();

    void log(LogEvent::ptr event) override;
private:
    
};


class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr; 

    FileLogAppender();
    ~FileLogAppender();
private:
    
};


# endif