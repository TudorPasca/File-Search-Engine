#pragma once

#include <string>
#include <utility>
#include <filesystem>

class FileDTO {
public:
    FileDTO(std::string  name, std::string  absolutePath, bool folder, std::string contents, std::string extension, uint64_t size_bytes);
    explicit FileDTO(const std::filesystem::directory_entry &entry);
    const std::string& getName() const;
    const std::string& getAbsolutePath() const;
    const bool isFolder() const;
    const std::string& getContents() const;
    std::string getExtension() const;
private:
    std::string name;
    std::string absolutePath;
    bool folder;
    std::string contents;
    std::string extension;
    uint64_t size_bytes;
};