#pragma once

#include <utility>
#include <vector>
#include "../DTO/FileDTO.h"
#include "../Repository/FileRepository.h"
#include "QueryParserService.h"

class SearchService {
public:
    SearchService(std::shared_ptr<FileRepository> repository, std::shared_ptr<QueryParserService> queryParser)
            : repository(std::move(repository)), queryParser(std::move(queryParser)) {}

    std::vector<FileDTO> getFiles(const std::string &searchQuery);

private:
    std::shared_ptr<FileRepository> repository;
    std::shared_ptr<QueryParserService> queryParser;
};