#pragma once

#include "IFileFilter.h"

class FileExtensionFilter: public IFileFilter {
public:
    explicit FileExtensionFilter(std::vector<std::string> extensions);
    [[nodiscard]] bool matches(const FileDTO &file) const override;
private:
    std::vector<std::string> extensions;
};