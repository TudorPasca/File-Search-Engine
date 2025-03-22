#pragma once

#include <utility>
#include <filesystem>
#include <fstream>
#include <pqxx/pqxx>
#include <iostream>
#include "FileScraper.h"

class IndexBuilder {
public:
    IndexBuilder() = default;

    explicit IndexBuilder(std::shared_ptr<IFileScraper> scraper, const std::string &connectionString) :
            scraper(std::move(scraper)), CONNECTION_STRING(connectionString) {
        logFile.open("index_log.txt", std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Error opening progress file!" << std::endl;
        }
    };

    void indexFiles(const std::filesystem::path &path);
private:
    const std::string CONNECTION_STRING;
    std::shared_ptr<IFileScraper> scraper;
    std::ofstream logFile;

    void writeToLog(const std::filesystem::path &path, std::vector<FileDTO> files);
};