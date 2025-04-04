#include "../../../include/utils/FileFilters/PathLengthFileFilter.h"

PathLengthFileFilter::PathLengthFileFilter(std::string::size_type length)
    : length(length) {}

bool PathLengthFileFilter::filter(const FileDTO &file) const {
    return file.getAbsolutePath().size() == length;
}