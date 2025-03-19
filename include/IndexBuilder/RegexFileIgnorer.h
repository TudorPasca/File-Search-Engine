#pragma once
#include "./IFileIgnorer.h"
#include <vector>
#include <regex>
#include <string>

class RegexFileIgnorer : public IFileIgnorer {
public:
    explicit RegexFileIgnorer(const std::vector<std::string>& patterns = {});
    bool shouldIgnore(const std::string& absolutePath) const override;
private:
    std::vector<std::regex> regexes;
};