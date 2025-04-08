#include <sstream>
#include <charconv>
#include "../../include/utils/FileFilters/IFileFilter.h"
#include "../../include/Service/QueryParserService.h"
#include "../../include/utils/FileFilters/PathKeywordFileFilter.h"
#include "../../include/utils/FileFilters/ContentFileFilter.h"
#include "../../include/utils/FileFilters/FileExtensionFilter.h"
#include "../../include/utils/FileFilters/PathLengthFileFilter.h"

std::vector<std::unique_ptr<IFileFilter>> QueryParserService::parseQuery(std::string_view query) const {
    std::vector<std::unique_ptr<IFileFilter>> filters;
    std::string query_str(query);
    std::istringstream iss(query_str);
    std::string segment;
    bool pathLengthSet = false;
    while (iss >> segment) {
        size_t colonPos = segment.find(':');
        if (colonPos == std::string::npos || colonPos == 0 || colonPos == segment.length() - 1) {
            throw std::invalid_argument("Invalid query format: " + segment);
        }
        std::string qualifier = segment.substr(0, colonPos);
        std::string value = segment.substr(colonPos + 1);
        if (qualifier == "path") {
            filters.push_back(std::make_unique<PathKeywordFileFilter>(value));
        } else if (qualifier == "content") {
            filters.push_back(std::make_unique<ContentFileFilter>(value));
        } else if (qualifier == "extension") {
            filters.push_back(std::make_unique<FileExtensionFilter>(value));
        } else if (qualifier == "length") {
            if (pathLengthSet) {
                throw std::invalid_argument("Duplicate path length constraint: " + segment);
            }
            size_t targetPathLength;
            pathLengthSet = true;
            const char* value_start = value.data();
            const char* value_end = value_start + value.size();
            auto result = std::from_chars(value_start, value_end, targetPathLength);
            if (result.ec != std::errc() || result.ptr != value_end) {
                throw std::invalid_argument("Invalid query format: " + segment);
            }
            filters.push_back(std::make_unique<PathLengthFileFilter>(targetPathLength));
        }
    }
    return filters;
}