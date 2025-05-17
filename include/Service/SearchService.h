#pragma once

#include <utility>
#include <vector>
#include "../DTO/FileDTO.h"
#include "../Repository/FileRepository.h"
#include "QueryParserService.h"
#include "SearchServiceCache/ISearchService.h"
#include "SearchSuggestions/ISearchObserver.h"

class SearchService: public ISearchService {
public:
    SearchService(std::shared_ptr<FileRepository> repository, std::shared_ptr<QueryParserService> queryParser)
            : repository(std::move(repository)), queryParser(std::move(queryParser)) {}

    void addObserver(std::shared_ptr<ISearchObserver> observer);
    void removeObserver(const std::shared_ptr<ISearchObserver>& observer);

    std::vector<FileDTO> getFiles(const std::string &searchQuery) override;

private:
    std::shared_ptr<FileRepository> repository;
    std::shared_ptr<QueryParserService> queryParser;
    std::vector<std::shared_ptr<ISearchObserver>> observers;

    void notifyObservers(const SearchEvent& event);
};