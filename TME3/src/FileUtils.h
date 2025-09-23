#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <functional>
#include <ios>

namespace pr {

/// @brief Partitions a text file into N roughly equal parts, ensuring word boundaries.
/// Useful for dividing work among threads without splitting words.
/// @param file Path to the text file.
/// @param S Total file size in bytes (must be accurate).
/// @param N Number of parts (must be > 0).
/// @return Vector of std::streamoff indices: [0, pos1, ..., S] defining N ranges.
std::vector<std::streamoff> partition(const std::string& file, std::streamoff S, int N);

/// @brief Prints word frequency results to a file.
/// Sorts pairs by frequency descending, then alphabetically.
/// @param total_words Total number of words processed.
/// @param unique_words Number of unique words.
/// @param freq_pairs Vector of (word, count) pairs.
/// @param filename Output file path (e.g., "mode.freq").
void printResults(size_t total_words, size_t unique_words, std::vector<std::pair<std::string, int>> freq_pairs, const std::string& filename);

/// @brief Cleans a raw word: removes non-letters, converts to lowercase.
/// @param raw The raw word string.
/// @return Cleaned word, or empty if no letters.
std::string cleanWord(const std::string &raw);

/// @brief Processes a byte range of a file, invoking a callback for each word.
/// Designed for efficient, range-based parsing (e.g., in multi-threading).
/// @param file Path to the text file.
/// @param start Starting byte offset (inclusive).
/// @param end Ending byte offset (exclusive).
/// @param onWordEncountered Callback function for each cleaned word.
void processRange(const std::string & file, std::streamoff start, std::streamoff end, std::function<void(const std::string&)> onWordEncountered);

} // namespace pr