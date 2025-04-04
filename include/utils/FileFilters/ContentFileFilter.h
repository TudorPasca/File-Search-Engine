#pragma once

#include "IFileFilter.h"

class ContentFileFilter: public IFileFilter {
public:
    explicit ContentFileFilter(std::vector<std::string> targetContents);
    [[nodiscard]] bool filter(const FileDTO& file) const override;
private:
    const std::vector<std::string> targetContents;
};