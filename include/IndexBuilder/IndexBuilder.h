#pragma once

#include <utility>
#include <filesystem>
#include <pqxx/pqxx>
#include "FileScraper.h"

class IndexBuilder {
public:
    IndexBuilder() = default;
    explicit IndexBuilder(std::shared_ptr<IFileScraper> scraper, const std::string &connectionString):
        scraper(std::move(scraper)), CONNECTION_STRING(connectionString) {};

    void indexFiles(const std::filesystem::path& path) const;
private:
    std::shared_ptr<IFileScraper> scraper;
    const std::string CONNECTION_STRING;
};