#pragma once

#include "../utils/FileFilters/IFileFilter.h"

class QueryParserService {
public:
    std::vector<std::unique_ptr<IFileFilter>> parseQuery(std::string_view query) const;
};