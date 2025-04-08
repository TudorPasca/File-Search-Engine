#pragma once

#include "IFileFilter.h"

class PathLengthFileFilter: public IFileFilter {
public:
    explicit PathLengthFileFilter(std::string::size_type length)
        : length(length) {}
    [[nodiscard]] const std::string::size_type getLength() const;
private:
    const std::string::size_type length;
};