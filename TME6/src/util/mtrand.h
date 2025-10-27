#pragma once
#include <limits>

namespace pr {
  // Thread-safe random number generator: [lo, hi), lo inclusive, hi exclusive
  int mtrand(int lo = 0, int hi = std::numeric_limits<int>::max());
}
