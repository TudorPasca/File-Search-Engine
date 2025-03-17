#include "../../include/IndexBuilder/FileScraper.h"

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

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
    for (const auto& it: fs::directory_iterator(entry, fs::directory_options::skip_permission_denied)) {
        if (ignorer != nullptr && ignorer->shouldIgnore(it.path().string()))
            continue;
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
    error_code err;
    for (const auto& it : fs::recursive_directory_iterator(entry, fs::directory_options::skip_permission_denied, err)) {
        if (err) {
            cerr << "Iterator error: " << err.message() << endl;
            err.clear();
            continue;
        }
        if (ignorer != nullptr && ignorer->shouldIgnore(it.path().string()))
            continue;
        files.emplace_back(it);
    }
    return files;
}

vector<FileDTO> FileScraper::getFilesRecursively(const fs::path& path) {
    fs::directory_entry entry{path};
    return getFilesRecursively(entry);
}