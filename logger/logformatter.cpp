#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "logformatter.h"
#include "loglevel.h"
#include "logevent.h"

// 
LogFormatter::LogFormatter(const std::string& pattern) : m_pattern(pattern) {
    init();
}

/*
%m 消息
%p 日志级别
%c 日志器名称
%d 日期时间，后面可跟一对括号指定时间格式，比如%d{%Y-%m-%d %H:%M:%S}，这里的格式字符与C语言strftime一致
%r 该日志器创建后的累计运行毫秒数
%f 文件名
%l 行号
%t 线程id
%F 协程id
%N 线程名称
%% 百分号
%T 制表符
%n 换行
*/
void LogFormatter::init(){
    // vec存储着解析到的各个pattern
    // int：表示类型，0 表示常规字符串，1 表示需要转义的模式字符。std::string：存储具体的字符串内容或模式字符。
    // 常规字符串是日志格式中直接输出的普通文本部分。它们不会被解析或转义，而是原样输出到日志中。
    // 模式字符是以 % 开头的特殊字符，用于表示需要动态替换的内容。它们通常用于插入日志的上下文信息，如时间、日志级别、消息等。
    std::vector<std::pair<int, std::string>> patterns;

    std::string temp;
    // 特别处理了日期格式（如 %d{...}），将其大括号内的内容提取出来并存储在 dateformat 中
    std::string date_str;

    bool error = false;
    // 是否在解析常规字符串（非pattern, %[a-z]）
    bool is_parsing_normal_string = true;
    size_t i = 0, n = m_pattern.size();
    while(i < n) {
        // 将 m_pattern[i] 这个字符转换为一个长度为1的 std::string 对象，并存储在变量 c 中
        std::string c(1, m_pattern[i]);
        // 
        if(c == "%") {
            if(is_parsing_normal_string) {
                // 如果当前正在解析常规字符串，那么将当前解析到的常规字符串存储到 vec 中
                // temp 不为空，将 temp 存储到 patterns 中，并清空 temp
                if(!temp.empty()) {
                    patterns.push_back(std::make_pair(0, temp));
                }
                temp.clear();
                is_parsing_normal_string = false;
                i++;
                continue;
            } else {
                // 如果当前正在解析 pattern，那么将当前解析到的 pattern 存储到 vec 中
                patterns.push_back(std::make_pair(1, temp));
                is_parsing_normal_string = true;
                i++;
                continue;
            }
        } 
        // c 不为 %，则将 c 添加到 temp 中
        else {
            if(is_parsing_normal_string) {
                temp += c;
                i++;
                continue;
            } else {
                // 将模板字符直接添加到 patterns 中
                patterns.push_back(std::make_pair(1, c));
                is_parsing_normal_string = true;

                // 特殊处理日期格式（如 %d{...}）
                if(c == "d") {
                    i++;
                    // 找到了 %d，尝试继续解析大括号内的内容
                    if(i < n && m_pattern[i] != '{') {
                        // 如果 %d 后面不是 {，则说明 %d 后面没有日期格式，直接跳过
                        continue;
                    }
                    // 找到{，开始解析日期格式
                    i++;
                    while(i < n && m_pattern[i] != '}') {
                        date_str += m_pattern[i];
                        i++;
                    }
                    if(m_pattern[i] != '}') {
                        // 如果没有找到 }，则说明日期格式不完整，解析失败
                        std::cout << "error: date format is not complete" << std::endl;
                        error = true;
                        break;
                    }
                } else {
                    i++;
                    continue;
                }
            }
        }
    }

    if(error) {
        return;
    }

    // 模板解析结束后的常规字符存储到 patterns 中
    if(!temp.empty()) {
        patterns.push_back(std::make_pair(0, temp));
        temp.clear();
    }

}

class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getLogContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << LogLevel::to_string(event->getLogLevel());
    }
};

class LoggerNameFormatItem : public LogFormatter::FormatItem {
public:
    LoggerNameFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getLoggerName();
    }
};

class FileNameFormatItem : public LogFormatter::FormatItem {
public:
    FileNameFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getFileName();
    }
};

class ElapseTimeFormatItem : public LogFormatter::FormatItem {
public:
    ElapseTimeFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getProgramRunnedTime();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

class CoroutineIdFormatItem : public LogFormatter::FormatItem {
public:
    CoroutineIdFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getCoroutineId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getThreadName();
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
public:
    // default format: %Y-%m-%d %H:%M:%S
    DateTimeFormatItem(const std::string& pattern = "%Y-%m-%d %H:%M:%S"): m_date_format(pattern){
        if(m_date_format.empty()) {
            m_date_format = "%Y-%m-%d %H:%M:%S";
        }
    }
    void format(std::ostream& os, const LogEvent::ptr event) override {
        time_t event_time_stamp = event->getTimeStamp();
        // 通过 localtime 将时间戳转换为 tm 结构体
        struct tm tm;
        localtime_r(&event_time_stamp, &tm);
        // 通过 strftime 将 tm 结构体转换为字符串
        char buf[64];
        strftime(buf, sizeof(buf), m_date_format.c_str(), &tm);
        os << buf;
    }
private:
    std::string m_date_format;
};

class FileLineFormatItem : public LogFormatter::FormatItem {
public:
    FileLineFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getFileName();
    }
};

class LineNumberFormatItem : public LogFormatter::FormatItem {
public:
    LineNumberFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << event->getLineNumber();
    }
};

// 将换行也作为一个格式化项
class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << std::endl;
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string& str): m_string(str) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << m_string;
    }
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << "\t";
    }
};

class PercentFormatItem : public LogFormatter::FormatItem {
public:
    PercentFormatItem(const std::string& pattern) {}
    void format(std::ostream& os, const LogEvent::ptr event) override {
        os << "%";
    }
};

std::ostream& LogFormatter::format(std::ostream& os, const LogEvent::ptr event) {
    for(auto& item : m_items) {
        item->format(os, event);
    }
    return os;
}

std::string LogFormatter::format(LogEvent::ptr event) {
    std::stringstream ss;
    for(auto& item : m_items) {
        item->format(ss, event);
    }
    return ss.str();
}