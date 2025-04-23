#include <iostream>
#include "../../include/Service/SearchService.h"

std::vector<FileDTO> SearchService::getFiles(const std::string &searchQuery) {
    std::vector<std::unique_ptr<IFileFilter>> filters = queryParser->parseQuery(searchQuery);
    SearchEvent event;
    event.query = searchQuery;
    event.timestamp = std::chrono::system_clock::now();
    notifyObservers(event);
    return repository->getFiles(filters);
}

void SearchService::addObserver(std::shared_ptr<ISearchObserver> observer) {
    if (!observer) return;
    observers.push_back(observer);
}

void SearchService::removeObserver(const std::shared_ptr<ISearchObserver>& observer) {
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void SearchService::notifyObservers(const SearchEvent& event) {
    for (const auto& observer : observers) {
        if (observer) {
            try {
                observer->onSearchPerformed(event);
            } catch (const std::exception& e) {
                std::cerr << "Error notifying observer: " << e.what() << std::endl;
            }
        }
    }
}