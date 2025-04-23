#pragma once

#include <memory>
#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include "../Service/SearchService.h"
#include "IController.h"
#include "../utils/FileFilters/IFileFilter.h"
#include "../Service/SearchSuggestions/SuggestionService.h"

class SearchController : public IController {
public:
    explicit SearchController(std::shared_ptr<SearchService> searchService,
                              std::shared_ptr<SuggestionService> suggestionService)
            : searchService(std::move(searchService)), suggestionService(std::move(suggestionService)) {}

    void registerRoutes(crow::App<crow::CORSHandler> &app) override;

private:
    std::shared_ptr<SearchService> searchService;
    std::shared_ptr<SuggestionService> suggestionService;
};