#include "../../include/IndexBuilder/FileScraper.h"

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

const fs::path FileScraper::BASE_PATH = R"(C:\)";

std::vector<FileDTO> FileScraper::getRegularFiles(const fs::directory_entry& entry) const {
    if (!entry.exists())
        return {};
    if (entry.is_regular_file()) {
        return {FileDTO(entry)};
    }
    std::vector<FileDTO> files;
    for (const auto& it: fs::directory_iterator(entry, fs::directory_options::skip_permission_denied)) {
        if (ignorer != nullptr && ignorer->shouldIgnore(it.path().string()))
            continue;
        if (it.is_regular_file()) {
            files.emplace_back(it);
        }
    }
    return files;
}

std::vector<FileDTO> FileScraper::getRegularFiles(const fs::path& path) const {
    fs::directory_entry entry{path};
    return getRegularFiles(entry);
}

std::vector<FileDTO> FileScraper::getFilesRecursively(const std::filesystem::directory_entry& entry) const {
    if (!entry.exists()) {
        return {};
    }
    if (entry.is_regular_file()) {
        return {FileDTO(entry)};
    }
    std::vector<FileDTO> files;
    std::error_code err;
    for (const auto& it : fs::recursive_directory_iterator(entry, fs::directory_options::skip_permission_denied, err)) {
        if (err) {
            std::cerr << "Iterator error: " << err.message() << std::endl;
            err.clear();
            continue;
        }
        if (ignorer != nullptr && ignorer->shouldIgnore(it.path().string()))
            continue;
        files.emplace_back(it);
    }
    return files;
}

std::vector<FileDTO> FileScraper::getFilesRecursively(const fs::path& path) const {
    fs::directory_entry entry{path};
    return getFilesRecursively(entry);
}