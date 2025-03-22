#pragma once

#include <vector>
#include <string>
#include "../DTO/FileDTO.h"

class FileRepository {
public:
    explicit FileRepository(const std::string &connectionString):
        CONNECTION_STRING(connectionString) {};
    std::vector<FileDTO> getFiles(const std::string &searchQuery);
private:
    const std::string CONNECTION_STRING;
};