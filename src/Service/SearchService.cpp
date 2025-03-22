#include "../../include/Service/SearchService.h"

std::vector<FileDTO> SearchService::getFiles(const std::string &searchQuery) {
    return repository->getFiles(searchQuery);
}