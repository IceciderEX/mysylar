#ifndef __LOG_FORMATTER_H__
#define __LOG_FORMATTER_H__

#include <string>
#include <memory>
#include "logevent.h"

/*
LogFormatter根据模板字符串来格式化日志事件，模板字符串中的格式符会被替换成对应的内容，格式符有：
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

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;

    LogFormatter(const std::string& pattern="%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
    ~LogFormatter() = default;
    // 格式化日志事件, 将日志事件event中的内容格式化到os中
    std::ostream& format(std::ostream& os, const LogEvent::ptr event);
    // 返回格式化后的日志内容
    std::string format(LogEvent::ptr event);
    void init();
public:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        virtual ~FormatItem() = default;
        // 格式化日志事件, 将日志事件event中的内容格式化到os中
        virtual void format(std::ostream& os, const LogEvent::ptr event) = 0;
    };

private:
    // 模板字符串
    std::string m_pattern;
    // 存储解析后的pattern
    std::vector<FormatItem::ptr> m_items;
};

#endif