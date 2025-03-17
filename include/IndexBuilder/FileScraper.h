#pragma once

#include <vector>
#include <string>
#include "../DTO/FileDTO.h"

class FileScraper {
public:
    static std::vector<FileDTO> getRegularFiles(const std::filesystem::directory_entry& entry);
    static std::vector<FileDTO> getRegularFiles(const std::filesystem::path& path = BASE_PATH);
    static std::vector<FileDTO> getFilesRecursively(const std::filesystem::directory_entry& entry);
    static std::vector<FileDTO> getFilesRecursively(const std::filesystem::path& path = BASE_PATH);
private:
    FileScraper() = default;
    static const std::filesystem::path BASE_PATH;
};