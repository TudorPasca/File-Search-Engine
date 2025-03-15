#include "../../include/IndexBuilder/FileScraper.h"

#include <string>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

const fs::path FileScraper::BASE_PATH = R"(C:\)";

vector<FileDTO> FileScraper::getRegularFiles(const fs::directory_entry& entry) {
    if (!entry.exists())
        return {};
    if (entry.is_regular_file()) {
        return {FileDTO(entry)};
    }
    vector<FileDTO> files;
    for (const auto& it: fs::directory_iterator(entry.path())) {
        if (it.is_regular_file()) {
            files.emplace_back(it);
        }
    }
    return files;
}

vector<FileDTO> FileScraper::getRegularFiles(const fs::path& path) {
    fs::directory_entry entry{path};
    return getRegularFiles(entry);
}

vector<FileDTO> FileScraper::getFilesRecursively(const std::filesystem::directory_entry& entry) {
    if (!entry.exists()) {
        return {};
    }
    if (entry.is_regular_file()) {
        return {FileDTO(entry)};
    }
    vector<FileDTO> files;
    for (const auto& it : fs::recursive_directory_iterator(entry)) {
        if (it.is_regular_file()) {
            files.emplace_back(it);
        }
    }
}

vector<FileDTO> FileScraper::getFilesRecursively(const fs::path& path) {
    fs::directory_entry entry{path};
    return getFilesRecursively(entry);
}