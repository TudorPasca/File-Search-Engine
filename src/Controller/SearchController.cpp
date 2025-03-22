#include "../../include/Controller/SearchController.h"

void SearchController::registerRoutes(crow::App<crow::CORSHandler> &app) {
    CROW_ROUTE(app, "/search")
            .methods("GET"_method)
                    ([this](const crow::request &req) {
                        char *searchQuery = req.url_params.get("q");
                        if (!searchQuery) {
                            return crow::response(400, "Missing search query parameter 'q'");
                        }
                        std::vector<FileDTO> results = this->searchService->getFiles(searchQuery);
                        crow::json::wvalue jsonList(crow::json::type::List);
                        size_t index = 0;
                        for (const auto &file : results) {
                            crow::json::wvalue fileJson(crow::json::type::Object);
                            fileJson["name"] = file.getName();
                            fileJson["path"] = file.getAbsolutePath();
                            fileJson["is_folder"] = file.isFolder();
                            fileJson["content"] = file.getContents();
                            jsonList[index++] = std::move(fileJson);
                        }
                        crow::json::wvalue jsonResponse;
                        jsonResponse["results"] = std::move(jsonList);
                        crow::response response(jsonResponse);
                        response.add_header("Access-Control-Allow-Origin", "http://localhost:4200");
                        response.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
                        response.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
                        return crow::response(jsonResponse);
                    });
}