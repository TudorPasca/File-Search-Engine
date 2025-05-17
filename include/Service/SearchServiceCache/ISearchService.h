#pragma once

#include <vector>
#include <string>
#include "../../DTO/FileDTO.h"

struct ISearchService {
    virtual ~ISearchService() = default;
    virtual std::vector<FileDTO> getFiles(const std::string &searchQuery) = 0;
};
