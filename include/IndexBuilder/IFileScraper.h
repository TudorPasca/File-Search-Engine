#pragma once

#include <vector>
#include "../DTO/FileDTO.h"

struct IFileScraper {
    virtual std::vector<FileDTO> getRegularFiles(const std::filesystem::directory_entry& entry) const = 0;
    virtual std::vector<FileDTO> getRegularFiles(const std::filesystem::path& path) const = 0;
    virtual std::vector<FileDTO> getFilesRecursively(const std::filesystem::directory_entry& entry) const = 0;
    virtual std::vector<FileDTO> getFilesRecursively(const std::filesystem::path& path) const = 0;
    virtual ~IFileScraper() = default;
};