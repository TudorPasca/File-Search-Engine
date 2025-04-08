#include "../../include/IndexBuilder/FileLogger.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include "../../include/IndexBuilder/ILogger.h"

FileLogger::FileLogger(const std::string &filename, LogLevel minLevel)
        : minLevel(minLevel) {
    logFileStream.open(filename, std::ios::app);
    if (!logFileStream.is_open()) {
        std::cerr << "Error opening progress file!" << std::endl;
    }
}

FileLogger::~FileLogger() {
    if (logFileStream.is_open()) {
        log(LogLevel::LOG_INFO, "Closing log file.");
        logFileStream.close();
    }
}

void FileLogger::log(LogLevel level, std::string_view message) {
    if (level < minLevel || !logFileStream.is_open())
        return;
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream time_ss;
    time_ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    logFileStream << "[" << time_ss.str() << "] "
                  << "[" << logLevelToString(level) << "] "
                  << message << std::endl;
}