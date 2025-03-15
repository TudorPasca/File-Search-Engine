#include "../../include/DTO/FileDTO.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

FileDTO::FileDTO(std::string name, std::string  absolutePath, bool folder, std::string contents):
name(std::move(name)), absolutePath(std::move(absolutePath)), folder(folder), contents(std::move(contents)) {}

FileDTO::FileDTO(const filesystem::directory_entry &entry) {
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
}

inline const string& FileDTO::getName() const { return name; }

inline const string& FileDTO::getAbsolutePath() const { return absolutePath; }

inline bool FileDTO::isFolder() const { return folder; }

inline const string& FileDTO::getContents() const { return contents; }