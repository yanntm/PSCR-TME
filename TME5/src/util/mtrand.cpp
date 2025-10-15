#include <random>
#include <cassert>
#include "util/mtrand.h"


// Thread-safe random number generator: [lo, hi), lo inclusive, hi exclusive
int pr::mtrand(int lo, int hi) {
    assert(lo < hi);
    static thread_local std::mt19937 generator{std::random_device{}()};
    return std::uniform_int_distribution<int>{lo, hi - 1}(generator);
}
