#pragma once

#include <string>
#include <vector>
#include <forward_list>
#include <utility>
#include <functional>

// un mot et son nombre d'occurrences
using WordCount = std::pair<std::string, int>;

// Table de hash dédiée au comptage : associe à chaque mot son nombre d'occurrences.
class FreqMap {
public:
    // Un bucket est une liste simplement chaînée de (mot, compteur).
    using Bucket = std::forward_list<WordCount>;

    // TODO : constructeur prenant le nombre de buckets

    // TODO : size_t size() const

    // TODO : void incrementFrequency(const std::string& word)

    // TODO : std::vector<WordCount> toKeyValuePairs() const

    // TODO (bonus) : void grow()

private:
    // TODO : les buckets, et le nombre d'entrées
};
