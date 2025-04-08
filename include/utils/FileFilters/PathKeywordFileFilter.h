#pragma once

#include "IFileFilter.h"

class PathKeywordFileFilter: public IFileFilter {
public:
    explicit PathKeywordFileFilter(std::vector<std::string> keywords);
    [[nodiscard]] bool matches(const FileDTO& file) const override;
private:
    const std::vector<std::string> keywords;
};