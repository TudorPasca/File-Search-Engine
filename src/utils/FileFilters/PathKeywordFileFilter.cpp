#include "../../../include/utils/FileFilters/PathKeywordFileFilter.h"

PathKeywordFileFilter::PathKeywordFileFilter(std::vector<std::string> keywords)
        : keywords(std::move(keywords)) {}

bool PathKeywordFileFilter::matches(const FileDTO &file) const {
    return std::all_of(keywords.begin(), keywords.end(), [file](const std::string& keyword) {
        return file.getAbsolutePath().find(keyword) != std::string::npos;
    });
}
