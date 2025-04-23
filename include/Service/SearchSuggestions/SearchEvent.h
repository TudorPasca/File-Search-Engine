#pragma once
#include <string>
#include <chrono>
#include <vector>

struct SearchEvent {
    std::string query;
    std::chrono::system_clock::time_point timestamp;
};