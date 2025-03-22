#pragma once

#include "../IndexBuilder/IndexBuilder.h"

class IndexService {
public:
    explicit IndexService(std::shared_ptr<IndexBuilder> indexBuilder):
        indexBuilder(std::move(indexBuilder)) {}
    void indexFiles(const std::string &path) const;

private:
    std::shared_ptr<IndexBuilder> indexBuilder;
};