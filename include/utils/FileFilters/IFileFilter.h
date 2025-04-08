#pragma once

#include "../../DTO/FileDTO.h"

struct IFileFilter {
    ///Returns TRUE if file passes filter
    virtual bool matches(const FileDTO& file) const = 0;
    virtual ~IFileFilter() = default;
};