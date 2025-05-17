#pragma once

#include <list>
#include <unordered_map>
#include "ICache.h"

template<typename K, typename V>
class LRUCache: public ICache<K, V> {
public:
    explicit LRUCache(const size_t capacity): capacity_(capacity) {}

    void insert(const K& key, const V& value) override {
        while (entries_.size() >= capacity_) {
            evict();
        }
        if (keyToEntry_.contains(key)) {
            typename std::list<std::pair<K, V>>::iterator it = keyToEntry_[key];
            entries_.erase(it);
            keyToEntry_.erase(key);
        }
        entries_.push_front(std::make_pair(key, value));
        keyToEntry_[key] = entries_.begin();
    }

    std::optional<V> get(const K& key) override {
        if (!keyToEntry_.contains(key)) {
            return std::nullopt;
        }
        typename std::list<std::pair<K, V>>::iterator it = keyToEntry_[key];
        std::pair<K, V> entry = *it;
        entries_.erase(it);
        entries_.push_front(entry);
        keyToEntry_[key] = entries_.begin();
        return entry.second;
    }
private:
    const size_t capacity_;
    std::list<std::pair<K, V>> entries_;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> keyToEntry_;

    void evict() {
        K lastEntryKey = entries_.back().first;
        keyToEntry_.erase(lastEntryKey);
        entries_.pop_back();
    }
};
