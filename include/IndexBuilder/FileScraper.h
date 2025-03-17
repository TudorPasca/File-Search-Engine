#pragma once

#include <vector>
#include <string>
#include "../DTO/FileDTO.h"
#include "IFileIgnorer.h"

class FileScraper {
public:
    FileScraper() = default;
    explicit FileScraper(std::shared_ptr<IFileIgnorer> ignorer = nullptr)
            : ignorer(ignorer) {}
    std::vector<FileDTO> getRegularFiles(const std::filesystem::directory_entry& entry);
    std::vector<FileDTO> getRegularFiles(const std::filesystem::path& path = BASE_PATH);
    std::vector<FileDTO> getFilesRecursively(const std::filesystem::directory_entry& entry);
    std::vector<FileDTO> getFilesRecursively(const std::filesystem::path& path = BASE_PATH);
private:
    static const std::filesystem::path BASE_PATH;
    std::shared_ptr<IFileIgnorer> ignorer;
};