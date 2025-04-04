#include "../../../include/utils/FileFilters/FileExtensionFilter.h"

FileExtensionFilter::FileExtensionFilter(std::vector<std::string> extensions)
        : extensions(std::move(extensions)) {}

bool FileExtensionFilter::filter(const FileDTO &file) const {
    std::string extension = file.getExtension();
    return std::any_of(extensions.begin(), extensions.end(), [&extension](const std::string &ext) {
        return ext == extension;
    });
}
