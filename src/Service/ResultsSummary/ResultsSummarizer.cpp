#include "../../../include/Service/ResultsSummary/ResultsSummarizer.h"

#include <numeric>
#include <unordered_map>

SummaryData ResultsSummarizer::generateSummary(const std::vector<FileDTO> &files) {
    return {getFileTypes(files), getAverageSize(files)};
}

AverageSizeSummary ResultsSummarizer::getAverageSize(const std::vector<FileDTO> &files) {
    if (files.empty()) {
        return AverageSizeSummary(0.0);
    }
    long long sizeSum = std::accumulate(files.begin(), files.end(), 0LL, [](long long sum, const FileDTO &file) {
        return sum + file.getSizeBytes();
    });
    double averageSize = static_cast<double>(sizeSum) / files.size();
    return AverageSizeSummary(averageSize);
}

std::vector<FileTypeSummary> ResultsSummarizer::getFileTypes(const std::vector<FileDTO> &files) {
    std::unordered_map<std::string, int> extensionCounts;
    for (const FileDTO &file: files) {
        std::string extension = file.getExtension();
        std::ranges::transform(extension, extension.begin(),
                               [](unsigned char c) { return std::tolower(c); });
        extensionCounts[extension]++;
    }
    std::vector<FileTypeSummary> fileTypes;
    fileTypes.reserve(extensionCounts.size());
    for (const auto &it: extensionCounts) {
        fileTypes.emplace_back(it.first, it.second);
    }
    return fileTypes;
}
