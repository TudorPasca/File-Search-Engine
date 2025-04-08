#pragma once

#include <vector>
#include <string>
#include "../DTO/FileDTO.h"
#include "../utils/FileFilters/IFileFilter.h"

class FileRepository {
public:
    explicit FileRepository(const std::string &connectionString):
        CONNECTION_STRING(connectionString) {};
    std::vector<FileDTO> getFiles(const std::string &searchQuery) const;
    std::vector<FileDTO> getFiles(const std::vector<std::unique_ptr<IFileFilter>> &filters) const;
private:
    const std::string CONNECTION_STRING;
};