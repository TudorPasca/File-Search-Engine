#include "../../../include/Service/SearchSuggestions/SearchHistoryTracker.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <pqxx/pqxx>

SearchHistoryTracker::SearchHistoryTracker(const std::string& connectionString)
        : dbConnectionString(connectionString) {}

void SearchHistoryTracker::persistSearchEvent(const SearchEvent& event) {
    try {
        pqxx::connection conn(dbConnectionString);
        pqxx::work txn(conn);
        std::string sql =
                "INSERT INTO public.search_history (query, timestamp) "
                "VALUES ($1, $2)";
        auto time = std::chrono::system_clock::to_time_t(event.timestamp);
        std::stringstream time_ss;
        time_ss << std::put_time(std::gmtime(&time), "%Y-%m-%dT%H:%M:%SZ");
        txn.exec_params(sql,
                        event.query,
                        time_ss.str());
        txn.commit();
    } catch (const pqxx::broken_connection& e) {
        std::cerr << "[SearchHistoryTracker] Database Connection Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[SearchHistoryTracker] Database Persistence Error: " << e.what() << std::endl;
    }
}

void SearchHistoryTracker::onSearchPerformed(const SearchEvent& event) {
    persistSearchEvent(event);
}