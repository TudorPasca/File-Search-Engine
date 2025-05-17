#pragma once
#include <unordered_map>

#include "ISearchService.h"
#include "Cache/ICache.h"

class SearchCacheProxy: public ISearchService {
public:
    SearchCacheProxy(std::shared_ptr<ICache<std::string, std::vector<FileDTO>>> cache, std::shared_ptr<ISearchService> searchService);

    std::vector<FileDTO> getFiles(const std::string &searchQuery) override;
private:
    std::shared_ptr<ICache<std::string, std::vector<FileDTO>>> cache_;
    std::shared_ptr<ISearchService> searchService_;
};
