#pragma once

#include "IFileFilter.h"

class PathKeywordFileFilter: public IFileFilter {
public:
    explicit PathKeywordFileFilter(std::vector<std::string> keywords);
    [[nodiscard]] bool filter(const FileDTO& file) const override;
private:
    const std::vector<std::string> keywords;
};