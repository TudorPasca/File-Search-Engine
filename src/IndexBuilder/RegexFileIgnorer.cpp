#include <filesystem>
#include "../../include/IndexBuilder/RegexFileIgnorer.h"

RegexFileIgnorer::RegexFileIgnorer(const std::vector<std::string>& patterns) {
    for (const auto& pattern : patterns) {
        regexes.emplace_back(pattern, std::regex::icase);
    }
}

bool RegexFileIgnorer::shouldIgnore(const std::string& absolutePath) const {
    std::filesystem::path filePath(absolutePath);
    if (!filePath.has_extension()) {
        return true;
    }
    for (const auto& regex : regexes) {
        if (std::regex_search(absolutePath, regex)) {
            return true;
        }
    }
    return false;
}