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
                            fileJson["score"] = file.getScore();
                            jsonList[index++] = std::move(fileJson);
                        }
                        crow::json::wvalue jsonResponse;
                        jsonResponse["results"] = std::move(jsonList);
                        return crow::response(jsonResponse);
                    });

    CROW_ROUTE(app, "/suggestions")
            .methods("GET"_method)
                    ([this](const crow::request& req) {
                        char* prefixQuery = req.url_params.get("p");
                        if (!prefixQuery) {
                            return crow::response(400, "Missing suggestion prefix parameter 'p'");
                        }
                        std::string prefixStr(prefixQuery);
                        int limit = 5;
                        std::vector<std::string> suggestions = this->suggestionService->getSuggestions(prefixStr, limit);
                        crow::json::wvalue jsonSuggestionList(crow::json::type::List);
                        size_t index = 0;
                        for (const auto& suggestion : suggestions) {
                            jsonSuggestionList[index++] = suggestion;
                        }
                        return crow::response(jsonSuggestionList);
                    });
}