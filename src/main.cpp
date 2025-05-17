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
#include "../include/Service/QueryParserService.h"
#include "../include/IndexBuilder/FileLogger.h"
#include "../include/IndexBuilder/FileScoringService.h"
#include "../include/Repository/SuggestionRepository.h"
#include "../include/Service/SearchServiceCache/SearchCacheProxy.h"
#include "../include/Service/SearchServiceCache/Cache/LRUCache.h"
#include "../include/Service/SearchSuggestions/SuggestionService.h"
#include "../include/Service/SearchSuggestions/SearchHistoryTracker.h"
#include "../include/Service/SpellingCorrection/ContentSpellingDecorator.h"
#include "../include/Service/SpellingCorrection/SpellingCorrectorService.h"

int main() {
    std::vector<std::shared_ptr<IController>> controllers;

    ///Indexing
    std::vector<std::string> ignorePatterns = {R"(^.*\.(?!(txt|json|cpp|c|h|py|java|xml|log)$)[A-Za-z0-9_]+$)"};
    auto ignorer = std::make_shared<RegexFileIgnorer>(ignorePatterns);
    auto fileScorer = std::make_shared<FileScoringService>();
    auto scraper = std::make_shared<FileScraper>(ignorer, fileScorer);

    auto logger = std::make_shared<FileLogger>(std::string("index_log.txt"), LogLevel::LOG_INFO);

    const std::string DB_CONNECTION_STRING = "host=localhost port=5432 dbname=file_search user=postgres password=123QWEasd";
    auto indexBuilder = std::make_shared<IndexBuilder>(logger, scraper, DB_CONNECTION_STRING);
    auto indexService = std::make_shared<IndexService>(indexBuilder);
    auto indexController = std::make_shared<IndexController>(indexService);
    controllers.push_back(indexController);

    ///Searching
    auto fileRepository = std::make_shared<FileRepository>(DB_CONNECTION_STRING);
    auto queryParserService = std::make_shared<QueryParserService>();
    auto searchService = std::make_shared<SearchService>(fileRepository, queryParserService);

    //Spell Correction Decorator
    auto spellingCorrector = std::make_shared<SpellingCorrectorService>("big.txt");
    auto contentCorrectionDecorator = std::make_shared<ContentSpellingDecorator>(searchService, spellingCorrector);

    //Cache Layer Proxy
    constexpr size_t CACHE_CAPACITY = 3;
    auto cache = std::make_shared<LRUCache<std::string, std::vector<FileDTO>>>(CACHE_CAPACITY);
    auto searchServiceProxy = std::make_shared<SearchCacheProxy>(cache, contentCorrectionDecorator);

    //Suggestions
    auto suggestionRepository = std::make_shared<SuggestionRepository>(DB_CONNECTION_STRING);
    auto suggestionService = std::make_shared<SuggestionService>(suggestionRepository);
    auto searchHistoryTracker = std::make_shared<SearchHistoryTracker>(DB_CONNECTION_STRING);
    searchService->addObserver(searchHistoryTracker);

    auto searchController = std::make_shared<SearchController>(searchServiceProxy, suggestionService);
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
