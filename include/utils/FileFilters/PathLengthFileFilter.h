#pragma once

#include "IFileFilter.h"

class PathLengthFileFilter: public IFileFilter {
public:
    explicit PathLengthFileFilter(std::string::size_type length);
    [[nodiscard]] bool filter(const FileDTO& file) const override;
private:
    const std::string::size_type length;
};