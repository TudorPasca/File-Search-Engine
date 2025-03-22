#pragma once

#include <memory>
#include <crow/app.h>
#include <crow/middlewares/cors.h>
#include "IController.h"
#include "../Service/IndexService.h"

class IndexController: public IController {
public:
    explicit IndexController(std::shared_ptr<IndexService> indexService):
        indexService(std::move(indexService)) {}
    void registerRoutes(crow::App<crow::CORSHandler> &app) override;
private:
    std::shared_ptr<IndexService> indexService;
};