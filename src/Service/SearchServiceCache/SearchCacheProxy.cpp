#include "../../../include/Service/SearchServiceCache/SearchCacheProxy.h"

SearchCacheProxy::SearchCacheProxy(std::shared_ptr<ICache<std::string, std::vector<FileDTO> > > cache, std::shared_ptr<ISearchService> searchService)
    : cache_(std::move(cache)), searchService_(std::move(searchService)) {}

std::vector<FileDTO> SearchCacheProxy::getFiles(const std::string &searchQuery) {
    std::optional<std::vector<FileDTO>> cacheFiles = cache_->get(searchQuery);
    if (cacheFiles)
        return cacheFiles.value();
    std::vector<FileDTO> files = searchService_->getFiles(searchQuery);
    cache_->insert(searchQuery, files);
    return files;
}
