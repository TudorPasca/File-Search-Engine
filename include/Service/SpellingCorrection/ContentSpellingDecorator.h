#pragma once
#include "SpellingCorrectorService.h"
#include "../SearchServiceCache/ISearchService.h"

class ContentSpellingDecorator: public ISearchService {
public:
    ContentSpellingDecorator(std::shared_ptr<ISearchService> searchService, std::shared_ptr<SpellingCorrectorService> spellingCorrector);
    std::vector<FileDTO> getFiles(const std::string &searchQuery) override;
private:
    std::shared_ptr<ISearchService> search_service_;
    std::shared_ptr<SpellingCorrectorService> spelling_corrector_;
    std::string correctContent(const std::string &query);
};