#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../../Repository/SuggestionRepository.h"

class SuggestionService {
public:
    explicit SuggestionService(std::shared_ptr<SuggestionRepository> repository);
    std::vector<std::string> getSuggestions(const std::string& prefix, int limit = 5);
private:
    std::shared_ptr<SuggestionRepository> suggestionRepository;
};