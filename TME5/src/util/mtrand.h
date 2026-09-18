#pragma once
#include <limits>

namespace pr {
  // Thread-safe random number generator: [lo, hi), lo inclusive, hi exclusive
  int mtrand(int lo = 0, int hi = std::numeric_limits<int>::max());
  // Reseed the calling thread's generator: the sequence of mtrand values becomes reproducible
  void mtseed(unsigned seed);
}
