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

int main() {
    ///Indexing
    const std::string DB_CONNECTION_STRING = "host=localhost port=5432 dbname=file_search user=postgres password=123QWEasd";
    std::vector<std::string> ignorePatterns = {R"(^.*\.(?!(txt|json|cpp|c|h|hpp)$)[A-Za-z0-9]+$)"};
    auto ignorer = std::make_shared<RegexFileIgnorer>(ignorePatterns);
    auto scraper = std::make_shared<FileScraper>(ignorer);
    IndexBuilder indexBuilder(scraper, DB_CONNECTION_STRING);
    indexBuilder.indexFiles(R"(C:\test)");

    ///Server
    auto fileRepository = std::make_shared<FileRepository>(DB_CONNECTION_STRING);
    auto searchService = std::make_shared<SearchService>(fileRepository);

    crow::App<crow::CORSHandler> app;
    auto &cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
            .origin("http://localhost:4200")
            .allow_credentials()
            .methods("POST"_method, "GET"_method);


    SearchController searchController(searchService);
    searchController.registerRoutes(app);
    app.port(18080).multithreaded().run();

    return 0;
}