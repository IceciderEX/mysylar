#ifndef __LOG_APPENDER_H__
#define __LOG_APPENDER_H__

/*
LogAppender是一个抽象类，定义了日志输出的接口，所有的日志输出类都需要继承LogAppender类，并实现其接口。
*/
class LogAppender {
public:
    LogAppender();
    virtual ~LogAppender();
private:
    
};


class StdoutLogAppender : public LogAppender {
public:
    StdoutLogAppender();
    ~StdoutLogAppender();
private:
    
};


class FileLogAppender : public LogAppender {
public:
    FileLogAppender();
    ~FileLogAppender();
private:
    
};


# endif