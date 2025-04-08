#pragma once

#include "ILogger.h"
#include <fstream>

class FileLogger: public ILogger {
public:
    explicit FileLogger(const std::string &filename, LogLevel minLevel = LogLevel::LOG_INFO);
    ~FileLogger() override;
private:
    std::ofstream logFileStream;
    LogLevel minLevel;
    void log(LogLevel level, std::string_view message) override;
};