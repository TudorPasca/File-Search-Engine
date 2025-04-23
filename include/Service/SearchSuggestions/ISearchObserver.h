#pragma once

#include "SearchEvent.h"

class ISearchObserver {
public:
    virtual ~ISearchObserver() = default;

    virtual void onSearchPerformed(const SearchEvent& event) = 0;
};