#include "../../include/IndexBuilder/FileScoringService.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <system_error>

namespace fs = std::filesystem;

namespace SimpleScoreParams {
    ///Factor Weights
    constexpr double W_RECENCY = 0.4;
    constexpr double W_PATH = 0.3;
    constexpr double W_SIZE = 0.3;

    ///Max Age Penalty ~= 3 months
    constexpr long long MAX_AGE_SECONDS = 90 * 24 * 60 * 60;

    constexpr double PATH_SCALE_FACTOR = 50.0;

    ///File Size Range
    constexpr uint64_t MIN_IDEAL_SIZE = 1024; ///1 KB
    constexpr uint64_t MAX_IDEAL_SIZE = 50 * 1024 * 1024; /// 50 MB
}

double FileScoringService::calculateScore(const fs::directory_entry& entry) const {
    double totalScore = 0.0;
    std::error_code ec;

    double recencyFactor = 0.0;
    double pathFactor = 0.0;
    double sizeFactor = 0.0;

    auto lastWriteTime = fs::last_write_time(entry, ec);
    if (!ec) {
        auto ageSeconds = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::file_clock::now() - lastWriteTime).count();
        if (ageSeconds >= 0 && ageSeconds <= SimpleScoreParams::MAX_AGE_SECONDS) {
            recencyFactor = 1.0 - (static_cast<double>(ageSeconds) / SimpleScoreParams::MAX_AGE_SECONDS);
        }
    }

    double pathLen = static_cast<double>(entry.path().string().length());
    pathFactor = SimpleScoreParams::PATH_SCALE_FACTOR / (SimpleScoreParams::PATH_SCALE_FACTOR + pathLen);

    uint64_t fileSize = fs::file_size(entry, ec);
    if (!ec) {
        if (fileSize >= SimpleScoreParams::MIN_IDEAL_SIZE && fileSize <= SimpleScoreParams::MAX_IDEAL_SIZE) {
            sizeFactor = 1.0;
        } else {
            sizeFactor = 0.1;
        }
    }

    totalScore = (SimpleScoreParams::W_RECENCY * recencyFactor) +
            (SimpleScoreParams::W_PATH * pathFactor) +
            (SimpleScoreParams::W_SIZE * sizeFactor);
    return std::max(0.0, std::min(1.0, totalScore));
}