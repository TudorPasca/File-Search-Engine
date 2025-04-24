#include "../../include/DTO/FileDTO.h"
#include <string>
#include <fstream>
#include <sstream>

namespace FileDTOConstants {
    constexpr std::streamsize PREVIEW_LENGTH = 200;
}

FileDTO::FileDTO(std::string name, std::string absolutePath, bool folder, std::string contents, std::string extension,
                 uint64_t sizeBytes) :
        name(std::move(name)), absolutePath(std::move(absolutePath)), folder(folder), contents(std::move(contents)),
        extension(std::move(extension)), sizeBytes(sizeBytes), score(0.0) {
    std::replace(this->absolutePath.begin(), this->absolutePath.end(), '\\', '/');
}

FileDTO::FileDTO(const std::filesystem::directory_entry &entry)
        : score(0.0) {
    name = entry.path().filename().string();
    absolutePath = entry.path().string();
    std::replace(absolutePath.begin(), absolutePath.end(), '\\', '/');
    folder = entry.is_directory();
    if (!folder) {
        std::ifstream file(absolutePath, std::ios::binary);
        if (file) {
            std::vector<char> buffer(FileDTOConstants::PREVIEW_LENGTH);
            file.read(buffer.data(), buffer.size());
            std::streamsize bytesRead = file.gcount();
            if (bytesRead > 0) {
                contents.assign(buffer.data(), bytesRead);
            }
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

const double FileDTO::getScore() const { return score; }

void FileDTO::setScore(double newScore) {
    this->score = newScore;
}