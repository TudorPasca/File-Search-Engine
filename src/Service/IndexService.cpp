#include "../../include/Service/IndexService.h"

void IndexService::indexFiles(const std::string &path) const {
    indexBuilder->indexFiles(path);
}