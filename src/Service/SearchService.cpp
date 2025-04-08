#include "../../include/Service/SearchService.h"

std::vector<FileDTO> SearchService::getFiles(const std::string &searchQuery) {
    std::vector<std::unique_ptr<IFileFilter>> filters = queryParser->parseQuery(searchQuery);
    return repository->getFiles(filters);
}