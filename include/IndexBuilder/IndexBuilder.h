#pragma once

#include <utility>
#include <filesystem>
#include <fstream>
#include <pqxx/pqxx>
#include <iostream>
#include "FileScraper.h"
#include "ILogger.h"

class IndexBuilder {
public:
    IndexBuilder() = default;

    explicit IndexBuilder(std::shared_ptr<ILogger> logger, std::shared_ptr<IFileScraper> scraper,
                          std::string connectionString) :
            logger(std::move(logger)), scraper(std::move(scraper)), CONNECTION_STRING(std::move(connectionString)) {};

    void indexFiles(const std::filesystem::path &path);

private:
    const std::string CONNECTION_STRING;
    std::shared_ptr<ILogger> logger;
    std::shared_ptr<IFileScraper> scraper;
};