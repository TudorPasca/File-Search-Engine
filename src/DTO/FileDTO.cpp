#include "../../include/DTO/FileDTO.h"
#include <string>
#include <fstream>
#include <sstream>

FileDTO::FileDTO(std::string name, std::string absolutePath, bool folder, std::string contents, std::string extension,
                 uint64_t sizeBytes) :
        name(std::move(name)), absolutePath(std::move(absolutePath)), folder(folder), contents(std::move(contents)),
        extension(std::move(extension)), sizeBytes(sizeBytes) {}

FileDTO::FileDTO(const std::filesystem::directory_entry &entry) {
    name = entry.path().filename().string();
    absolutePath = entry.path().string();
    folder = entry.is_directory();
    if (!folder) {
        std::ifstream file(absolutePath);
        if (file) {
            std::ostringstream oss;
            oss << file.rdbuf();
            contents = oss.str();
        }
    }
    size_t extensionDot = absolutePath.rfind('.');
    if (!folder && extensionDot != std::string::npos && extensionDot != 0 && extensionDot != absolutePath.size() - 1) {
        extension = absolutePath.substr(extensionDot + 1);
    }
    sizeBytes = std::filesystem::file_size(entry);
}

const std::string &FileDTO::getName() const { return name; }

const std::string &FileDTO::getAbsolutePath() const { return absolutePath; }

const bool FileDTO::isFolder() const { return folder; }

const std::string &FileDTO::getContents() const { return contents; }

const std::string FileDTO::getExtension() const { return extension; }

const uint64_t FileDTO::getSizeBytes() const { return sizeBytes; }