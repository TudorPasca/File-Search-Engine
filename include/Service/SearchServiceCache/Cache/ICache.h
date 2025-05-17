#pragma once

#include <optional>

template<typename K, typename V>
struct ICache {
    virtual ~ICache() = default;
    virtual void insert(const K&, const V&) = 0;
    virtual std::optional<V> get(const K&) = 0;
};