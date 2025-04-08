#pragma once

#include "IFileFilter.h"

class FileExtensionFilter: public IFileFilter {
public:
    explicit FileExtensionFilter(std::string extension)
        : extension(std::move(extension)) {}
    [[nodiscard]] const std::string& getExtension() const;
private:
    std::string extension;
};