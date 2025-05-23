#pragma once
#include <string>
#include <vector>

struct FileTypeSummary {
    FileTypeSummary(std::string extension, const int count): extension(std::move(extension)), count(count) {
    }

    std::string extension;
    int count;
};

struct AverageSizeSummary {
    explicit AverageSizeSummary(const double averageSize): averageSize(averageSize) {
    }

    double averageSize;
};

struct SummaryData {
    SummaryData(const std::vector<FileTypeSummary> &fileTypes, const AverageSizeSummary averageSize)
        : fileTypes(fileTypes), averageSize(averageSize) {
    }

    std::vector<FileTypeSummary> fileTypes;
    AverageSizeSummary averageSize;
};
