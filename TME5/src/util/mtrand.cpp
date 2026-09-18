#include <random>
#include <cassert>
#include "util/mtrand.h"

namespace {
// One generator per thread, seeded from the system entropy unless mtseed is called
std::mt19937 &generator() {
    static thread_local std::mt19937 gen{std::random_device{}()};
    return gen;
}
}

// Thread-safe random number generator: [lo, hi), lo inclusive, hi exclusive
int pr::mtrand(int lo, int hi) {
    assert(lo < hi);
    return std::uniform_int_distribution<int>{lo, hi - 1}(generator());
}

void pr::mtseed(unsigned seed) {
    generator().seed(seed);
}
