#include "../../include/IndexBuilder/RegexFileIgnorer.h"

using namespace std;

RegexFileIgnorer::RegexFileIgnorer(const std::vector<std::string>& patterns) {
    for (const auto& pattern : patterns) {
        regexes.emplace_back(pattern, regex::icase);
    }
}

bool RegexFileIgnorer::shouldIgnore(const std::string& absolutePath) const {
    for (const auto& regex : regexes) {
        if (std::regex_search(absolutePath, regex)) {
            return true;
        }
    }
    return false;
}