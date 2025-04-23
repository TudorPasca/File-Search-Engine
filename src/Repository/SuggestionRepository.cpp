#include "../../include/Repository/SuggestionRepository.h"
#include <vector>
#include <string>
#include <iostream>
#include <pqxx/pqxx>

SuggestionRepository::SuggestionRepository(const std::string& connectionString)
        : dbConnectionString(connectionString) {}

std::vector<std::string> SuggestionRepository::findSuggestionsByPrefix(const std::string& prefix, int limit) const {
    std::vector<std::string> suggestions;
    if (prefix.empty() || limit <= 0) {
        return suggestions;
    }
    try {
        pqxx::connection conn(dbConnectionString);
        pqxx::work txn(conn);
        std::string sql =
                "SELECT DISTINCT query "
                "FROM public.search_history "
                "WHERE query LIKE $1 || '%' "
                "ORDER BY timestamp DESC "
                "LIMIT $2";
        pqxx::result res = txn.exec_params(sql, prefix, limit);
        suggestions.reserve(res.size());
        for (const auto& row : res) {
            suggestions.push_back(row[0].as<std::string>());
        }
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "[SuggestionRepository] Database Connection Error: " << e.what() << std::endl;
        return {};
    }
    catch (const std::exception& e) {
        std::cerr << "[SuggestionRepository] Database Query Error: " << e.what() << std::endl;
        return {};
    }
    return suggestions;
}