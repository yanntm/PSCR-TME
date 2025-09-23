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
        // todo

    };

    using Bucket = std::forward_list<Entry>;
    using Table  = std::vector<Bucket>;

    // Construct with a number of buckets (must be >= 1)
    // HashMap(std::size_t nbuckets = 1024);

    // Return pointer to value associated with key, or nullptr if not found.
    // Only iterate the appropriate bucket.
    // V* get(const K& key);

    // Insert or update (key,value).
    // Returns true if an existing entry was updated, false if a new entry was inserted.
    // bool put(const K& key, const V& value);

    // Current number of stored entries
    // std::size_t size() const;

    // Convert table contents to a vector of key/value pairs.
    // std::vector<std::pair<K,V>> toKeyValuePairs() const;

    // Optional: number of buckets
    // std::size_t bucket_count() const;

private:
    Table buckets_;
    std::size_t count_ = 0;
};
