#ifndef __LOGLEVEL_H__
#define __LOGLEVEL_H__

#include <string>
// enum LOG_LEVEL {
//     LOG_LEVEL_DEBUG = 0,
//     LOG_LEVEL_INFO,
//     LOG_LEVEL_WARN,
//     LOG_LEVEL_ERROR,
//     LOG_LEVEL_FATAL,
//     LOG_LEVEL_NONE
// };

class LogLevel {
public:
    enum Level {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        NONE
    };

    static std::string to_string(LogLevel::Level level);
    static LogLevel::Level from_string(const std::string& str);
};


#endif