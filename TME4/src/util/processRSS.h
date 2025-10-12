// process.hpp
#pragma once

#include <cstddef> // for size_t
#include <iosfwd> // for std::ostream

namespace process {

/**
 * Struct representing resident set size (RSS) metrics.
 * RSS is the portion of memory occupied by a process that is held in main memory (RAM).
 * This is process-wide RSS (not per-thread or heap-only).
 * - current: Current RSS in bytes.
 * - peak: Peak RSS (high-water mark) in bytes.
 * The measurement is platform-specific and may return {0,0} on unsupported systems or errors.
 * Usage example:
 * \code
 * MemRSS rss = getResidentMemory();
 * std::cout << "Current RSS: " << rss.current << " bytes\n";
 * std::cout << "Peak RSS: " << rss.peak << " bytes\n";
 * \endcode
 */
struct MemRSS {
    size_t current;
    size_t peak;

    friend std::ostream& operator<<(std::ostream& os, const MemRSS& m); // Prints a human-readable representation of the metrics.
};

/**
 * Returns current and peak resident set size (RSS) in bytes.
 * RSS is the portion of memory occupied by a process that is held in main memory (RAM).
 * This is process-wide RSS (not per-thread or heap-only).
 * The measurement is platform-specific and may return {0,0} on unsupported systems or errors.
 */
MemRSS getResidentMemory();

} // namespace process