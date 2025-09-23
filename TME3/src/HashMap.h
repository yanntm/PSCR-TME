#pragma once

#include <vector>
#include <forward_list>
#include <utility>
#include <cstddef>

template<typename K, typename V>
class HashMap {
public:
    // Entry stores a const key and a mutable value
    struct Entry {
        const K key;
        V value;
        Entry(const K& k, const V& v) : key(k), value(v) {}
    };

    using Bucket = std::forward_list<Entry>;
    using Table  = std::vector<Bucket>;

    // Construct with a number of buckets (must be >= 1)
    HashMap(std::size_t nbuckets = 4096) : buckets_(nbuckets) {}

    // Increment frequency for the given word
    void incrementFrequency(const K& key, V delta = 1) {
        std::size_t idx = std::hash<K>{}(key) % buckets_.size();
        for (Entry &e : buckets_[idx]) {
            if (e.key == key) { e.value += delta; return; }
        }
        buckets_[idx].emplace_front(key, delta);
    }

    // Convert table contents to a vector of key/value pairs.
    std::vector<std::pair<K,V>> toKeyValuePairs() const {
        std::vector<std::pair<K,V>> out;
        for (const auto &bucket : buckets_) {
            for (const auto &e : bucket) {
                out.emplace_back(e.key, e.value);
            }
        }
        return out;
    }

private:
    Table buckets_;
};
