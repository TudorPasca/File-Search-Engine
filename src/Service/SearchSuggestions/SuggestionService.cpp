#include "../../../include/Service/SearchSuggestions/SuggestionService.h"
#include <vector>
#include <string>

SuggestionService::SuggestionService(std::shared_ptr<SuggestionRepository> repository)
        : suggestionRepository(std::move(repository)) {}

std::vector<std::string> SuggestionService::getSuggestions(const std::string& prefix, int limit) {
    return suggestionRepository->findSuggestionsByPrefix(prefix, limit);
}