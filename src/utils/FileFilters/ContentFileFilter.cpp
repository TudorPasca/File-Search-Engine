#include "../../../include/utils/FileFilters/ContentFileFilter.h"

ContentFileFilter::ContentFileFilter(std::vector<std::string> targetContents)
        : targetContents(std::move(targetContents)) {}

bool ContentFileFilter::filter(const FileDTO &file) const {
    return std::all_of(targetContents.begin(), targetContents.end(), [file](const std::string &content) {
        return file.getContents().find(content) != std::string::npos;
    });
}