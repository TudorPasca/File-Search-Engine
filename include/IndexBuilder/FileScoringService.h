#pragma once

#include "IFileScoringService.h"

class FileScoringService : public IFileScoringService {
public:
    FileScoringService() = default;

    double calculateScore(const fs::directory_entry& entry) const override;
};