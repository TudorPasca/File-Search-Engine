#pragma once
#include "SummaryData.h"
#include "../../DTO/FileDTO.h"

class ResultsSummarizer {
public:
    static SummaryData generateSummary(const std::vector<FileDTO>& files);

private:
    static AverageSizeSummary getAverageSize(const std::vector<FileDTO>& files);
    static std::vector<FileTypeSummary> getFileTypes(const std::vector<FileDTO>& files);
};
