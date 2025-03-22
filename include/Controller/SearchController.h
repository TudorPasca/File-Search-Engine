#pragma once

#include <memory>
#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include "../Service/SearchService.h"
#include "IController.h"

class SearchController: public IController {
public:
    explicit SearchController(std::shared_ptr<SearchService> searchService):
        searchService(std::move(searchService)) {}
    void registerRoutes(crow::App<crow::CORSHandler> &app) override;
private:
    std::shared_ptr<SearchService> searchService;
};