#include <string>
#include "loglevel.h"

std::string LogLevel::to_string(LogLevel::Level level) {
    switch(level) {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        case NONE:
            return "NONE";
        default:
            return "UNKNOWN";
    }
}

LogLevel::Level LogLevel::from_string(const std::string& str) {
    if(str == "DEBUG") {
        return DEBUG;
    } else if(str == "INFO") {
        return INFO;
    } else if(str == "WARN") {
        return WARN;
    } else if(str == "ERROR") {
        return ERROR;
    } else if(str == "FATAL") {
        return FATAL;
    } else if(str == "NONE") {
        return NONE;
    } else {
        return NONE;
    }
}