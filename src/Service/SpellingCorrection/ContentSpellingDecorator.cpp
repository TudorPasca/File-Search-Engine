#include "../../../include/Service/SpellingCorrection/ContentSpellingDecorator.h"

#include <regex>
#include <iostream>

ContentSpellingDecorator::ContentSpellingDecorator(std::shared_ptr<ISearchService> searchService,
                                                   std::shared_ptr<SpellingCorrectorService> spellingCorrector)
    : search_service_(std::move(searchService)), spelling_corrector_(std::move(spellingCorrector)) {
}

std::vector<FileDTO> ContentSpellingDecorator::getFiles(const std::string &searchQuery) {
        std::string correctQuery = std::move(correctContent(searchQuery));
        return search_service_->getFiles(correctQuery);
}


std::string ContentSpellingDecorator::correctContent(const std::string &query) {
    std::regex pattern(R"((.*?)(content:)([^ ]+)(.*))", std::regex_constants::icase);
    std::smatch match;
    if (std::regex_search(query, match, pattern) && match.size() == 5) {
        std::string original_value = match[3].str();
        if (original_value.empty()) {
            return query;
        }
        std::string value_to_correct_lower = original_value;
        std::transform(value_to_correct_lower.begin(), value_to_correct_lower.end(),
                       value_to_correct_lower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::string corrected_value_lower = spelling_corrector_->correct(value_to_correct_lower);
        if (corrected_value_lower != value_to_correct_lower) {
            std::cout << "[ContentSpellingDecorator] Corrected " << value_to_correct_lower << " to " << corrected_value_lower << std::endl;
            return match[1].str() +
                   match[2].str() +
                   corrected_value_lower +
                   match[4].str();
        }
    }
    return query;
}
