#pragma once
#include <string>

struct IFileIgnorer {
    virtual bool shouldIgnore(const std::string& absolutePath) const = 0;
    virtual ~IFileIgnorer() = default;
};