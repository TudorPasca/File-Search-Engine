#pragma once

#include <string>
#include <utility>
#include <filesystem>

class FileDTO {
public:
    FileDTO(std::string  name, std::string  absolutePath, bool folder, std::string contents);
    explicit FileDTO(const std::filesystem::directory_entry &entry);
    const std::string& getName() const;
    const std::string& getAbsolutePath() const;
    const bool isFolder() const;
    const std::string& getContents() const;
private:
    std::string name;
    std::string absolutePath;
    bool folder;
    std::string contents;
};