#pragma once

#include <string>
#include <memory>
#include <vector>

class SuggestionRepository {
public:
    explicit SuggestionRepository(const std::string &connectionString);

    std::vector<std::string> findSuggestionsByPrefix(const std::string &prefix, int limit) const;
private:
    std::string dbConnectionString;
};