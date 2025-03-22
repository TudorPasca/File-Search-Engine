
#include "../../include/Controller/IndexController.h"

void IndexController::registerRoutes(crow::App<crow::CORSHandler> &app) {
    CROW_ROUTE(app, "/index")
            .methods("POST"_method)
                    ([this](const crow::request &req) {
                        auto body = crow::json::load(req.body);
                        if (!body || !body.has("path")) {
                            return crow::response(400, "Invalid request format");
                        }
                        std::string path = body["path"].s();
                        indexService->indexFiles(path);
                        return crow::response(200, "Indexing started successfully");
                    });
}