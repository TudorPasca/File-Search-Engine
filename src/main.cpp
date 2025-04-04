#include <iostream>
#include <vector>
#include <string>
#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include "../include/IndexBuilder/FileScraper.h"
#include "../include/IndexBuilder/RegexFileIgnorer.h"
#include "../include/IndexBuilder/IndexBuilder.h"
#include "../include/Repository/FileRepository.h"
#include "../include/Service/SearchService.h"
#include "../include/Controller/SearchController.h"
#include "../include/Service/IndexService.h"
#include "../include/Controller/IndexController.h"

int main() {
    std::vector<std::shared_ptr<IController>> controllers;

    ///Indexing
    const std::string DB_CONNECTION_STRING = "host=localhost port=5432 dbname=file_search user=postgres password=123QWEasd";
    std::vector<std::string> ignorePatterns = {R"(^.*\.(?!(txt|json|cpp|c|h|py|java|in|out)$)[A-Za-z0-9]+$)"};
    auto ignorer = std::make_shared<RegexFileIgnorer>(ignorePatterns);
    auto scraper = std::make_shared<FileScraper>(ignorer);
    auto indexBuilder = std::make_shared<IndexBuilder>(scraper, DB_CONNECTION_STRING);
    auto indexService = std::make_shared<IndexService>(indexBuilder);
    auto indexController = std::make_shared<IndexController>(indexService);
    controllers.push_back(indexController);

    ///Searching
    auto fileRepository = std::make_shared<FileRepository>(DB_CONNECTION_STRING);
    auto searchService = std::make_shared<SearchService>(fileRepository);
    auto searchController = std::make_shared<SearchController>(searchService);
    controllers.push_back(searchController);

    crow::App<crow::CORSHandler> app;
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
            .origin("http://localhost:4200")
            .allow_credentials()
            .methods("POST"_method, "GET"_method);

    for (const auto &controller: controllers) {
        controller->registerRoutes(app);
    }

    app.port(18080).multithreaded().run();

    return 0;
}