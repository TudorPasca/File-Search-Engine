#include "../../include/Controller/SearchController.h"
#include "../../include/Service/ResultsSummary/ResultsSummarizer.h"

void SearchController::registerRoutes(crow::App<crow::CORSHandler> &app) {
CROW_ROUTE(app, "/search")
    .methods("GET"_method)
        ([this](const crow::request &req) {
            char *searchQueryChars = req.url_params.get("q");
            if (!searchQueryChars) {
                return crow::response(400, "Missing search query parameter 'q'");
            }
            std::string searchQuery(searchQueryChars);

            std::vector<FileDTO> results = this->searchService->getFiles(searchQuery);
            crow::json::wvalue jsonResultsList(crow::json::type::List);
            size_t index = 0;
            for (const auto &file : results) {
                crow::json::wvalue fileJson(crow::json::type::Object);
                fileJson["name"] = file.getName();
                fileJson["path"] = file.getAbsolutePath();
                fileJson["is_folder"] = file.isFolder();
                fileJson["content"] = file.getContents();
                fileJson["score"] = file.getScore();
                jsonResultsList[index++] = std::move(fileJson);
            }

            SummaryData summaryData = ResultsSummarizer::generateSummary(results);
            crow::json::wvalue jsonSummaries(crow::json::type::Object);
            crow::json::wvalue jsonFileTypesList(crow::json::type::List);
            size_t ft_idx = 0;
            for (const auto &fileTypeSummary : summaryData.fileTypes) {
                crow::json::wvalue item(crow::json::type::Object);
                item["extension"] = fileTypeSummary.extension;
                item["count"] = fileTypeSummary.count;
                jsonFileTypesList[ft_idx++] = std::move(item);
            }
            jsonSummaries["fileTypes"] = std::move(jsonFileTypesList);
            jsonSummaries["averageFileSize"] = summaryData.averageSize.averageSize;

            crow::json::wvalue jsonResponse;
            jsonResponse["results"] = std::move(jsonResultsList);
            jsonResponse["summaries"] = std::move(jsonSummaries);
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