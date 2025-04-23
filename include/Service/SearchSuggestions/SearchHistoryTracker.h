#pragma once

#include "ISearchObserver.h"
#include <memory>
class SearchHistoryTracker : public ISearchObserver {
public:
    explicit SearchHistoryTracker(const std::string& connectionString);

    void onSearchPerformed(const SearchEvent& event) override;
private:
    std::string dbConnectionString;

    void persistSearchEvent(const SearchEvent& event);
};