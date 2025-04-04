#pragma once

#include "../../DTO/FileDTO.h"

struct IFileFilter {
    ///Returns TRUE if file passes filter
    virtual bool filter(const FileDTO& file) const = 0;
    virtual ~IFileFilter() = default;
};