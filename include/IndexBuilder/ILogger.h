#pragma once

#include <string_view>

enum class LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct ILogger {
    virtual ~ILogger() = default;

    void logInfo(std::string_view message) {
        log(LogLevel::LOG_INFO, message);
    }

    void logWarning(std::string_view message) {
        log(LogLevel::LOG_WARNING, message);
    }

    void logError(std::string_view message) {
        log(LogLevel::LOG_ERROR, message);
    }
private:
    virtual void log(LogLevel level, std::string_view message) = 0;
};

inline std::string_view logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::LOG_INFO:
            return "INFO";
        case LogLevel::LOG_WARNING:
            return "WARNING";
        case LogLevel::LOG_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}