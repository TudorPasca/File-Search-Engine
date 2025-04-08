#pragma once

#include "IFileFilter.h"

class PathKeywordFileFilter: public IFileFilter {
public:
    explicit PathKeywordFileFilter(std::string keyword)
        : keyword(std::move(keyword)) {}
    [[nodiscard]] const std::string& getKeyword() const;
private:
    const std::string keyword;
};