#pragma once

#include "IFileFilter.h"

class ContentFileFilter: public IFileFilter {
public:
    explicit ContentFileFilter(std::string content)
        : content(std::move(content)) {}
    [[nodiscard]] const std::string& getContent() const;
private:
    const std::string content;
};