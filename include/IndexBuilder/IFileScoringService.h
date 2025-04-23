#pragma once

#include <filesystem>
#include <memory>

namespace fs = std::filesystem;

class IFileScoringService {
public:
    virtual ~IFileScoringService() = default;

    virtual double calculateScore(const fs::directory_entry& entry) const = 0;
};