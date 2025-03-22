#pragma once

#include <utility>
#include <vector>
#include "../DTO/FileDTO.h"
#include "../Repository/FileRepository.h"

class SearchService {
public:
    SearchService(std::shared_ptr<FileRepository> repository):
        repository(std::move(repository)) {}
    std::vector<FileDTO> getFiles(const std::string &searchQuery);

private:
    std::shared_ptr<FileRepository> repository;
};