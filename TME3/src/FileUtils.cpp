#include <vector>
#include <string>
#include <fstream> // for file I/O
#include <iostream>
#include <cctype>  // for isspace
#include <regex>
#include <sstream> // for stringstream
#include <cassert> // For assert
#include <ios> // For std::streamsize
#include "FileUtils.h"

// partition: Divides a text file into N roughly equal-sized parts (by byte count),
// adjusting boundaries to word edges to avoid splitting words across parts.
// Crucial for multi-threaded processing without word fragmentation.
//
// How it works:
// - Divide file size S into N parts, targeting i*S/N for boundaries.
// - At each target, if mid-word, advance to end of word.
// - Returns vector of N+1 offsets: [0, pos1, ..., S]
// - Parts are [offsets[k], offsets[k+1]) for k=0 to N-1.
//
// Parameters:
// - file: Path to the text file.
// - S: Total file size in bytes.
// - N: Number of parts (>0).
//
// Returns: Vector of offsets. Degenerate cases (N<=0, S<=0, S<N) return {0, S}.
std::vector<std::streamoff> pr::partition(const std::string& file, std::streamoff S, int N) {
    // Assert basic validity (disabled in release with -DNDEBUG)
    assert(!file.empty() && "File path must not be empty");
    assert(S >= 0 && "File size must be non-negative");
    assert(N > 0 && "Number of parts must be positive");

    if (S <= 0 || S < N) {
        return {0, S};
    }

    std::vector<std::streamoff> indices{0};

    // Open in binary mode for accurate positioning.
    std::ifstream in(file, std::ios::binary);

    for (int i = 1; i < N; ++i) {
        std::streamoff target = i * S / N;
        in.seekg(target);
        in.clear();

        int c = in.peek();
        if (c != std::char_traits<char>::eof() && !std::isspace(static_cast<unsigned char>(c))) {
            // Mid-word or at start: Advance to next boundary.
            std::string dummy;
            in >> dummy;
        }

        in.clear();
        std::streamoff pos = in.tellg();
        if (pos == -1) {
            pos = S;
        }

        indices.push_back(pos);
    }

    indices.push_back(S);
    return indices;
}

void pr::printResults(size_t total_words, size_t unique_words, std::vector<std::pair<std::string, int>> freq_pairs, const std::string& filename) {
    std::sort(freq_pairs.begin(), freq_pairs.end(), [](const auto &a, const auto &b) { 
        return a.second > b.second || (a.second == b.second && a.first < b.first); 
    });
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening output file: " << filename << std::endl;
        return;
    }
    out << total_words << std::endl;
    out << unique_words << std::endl;
    for (const auto& p : freq_pairs) {
        out << p.second << " " << p.first << std::endl;
    }
}

std::string pr::cleanWord(const std::string &raw) {
    // une regex qui reconnait les caractères anormaux (négation des lettres)
    static const std::regex re(R"([^a-zA-Z])");
    // élimine la ponctuation et les caractères spéciaux
    std::string w = std::regex_replace(raw, re, "");
    // passe en lowercase
    std::transform(w.begin(), w.end(), w.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return w;
}

// processRange: Efficiently processes a byte range [start, end) of a text file,
// reading words and invoking a callback for each valid word.
// Designed for multi-threaded ranges from partition().
//
// Key features:
// - Binary mode for consistent positioning.
// - Buffered reads with backtracking for word integrity.
// - Cleans words via pr::cleanWord.
//
// Parameters:
// - file: Text file path.
// - start/end: Byte range (inclusive/exclusive).
// - onWordEncountered: Callback for valid words.
//
// Notes: Assumes short words and aligned ranges; ignores unsafe signs warnings.
void pr::processRange(const std::string & file, std::streamoff start, std::streamoff end, std::function<void(const std::string&)> onWordEncountered) {
    // Assert args (disabled in release via -DNDEBUG).
    assert(!file.empty() && "File path must not be empty");
    assert(start >= 0 && "Start offset must be non-negative");
    assert(end >= start && "End offset must be at least start");
    std::ifstream in(file, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening file: " << file << std::endl;
        return;
    }
    in.seekg(start);
    // Max block size (signed for stream compat).
    // a bit arbitrary, could be tuned. 
    // Larger sizes use more memory but reduce system I/O calls potentially.
    const std::streamsize BLOCK_SIZE = 2048;
    // Reusable buffer (avoids per-loop allocs).
    std::string buffer(BLOCK_SIZE, '\0');
    // Current file pos.
    std::streamoff current_pos = start;
    while (current_pos < end) {
        // Remaining bytes in range.
        std::streamoff remaining = end - current_pos;
        // To read: Min fits last block.
        std::streamsize to_read = std::min(BLOCK_SIZE, static_cast<std::streamsize>(remaining));
        // std::cerr << "current_pos=" << current_pos << ", remaining=" << remaining << ", to_read=" << to_read << std::endl;
        buffer.resize(to_read); // Resize to to_read (may warn signed/unsigned, safe).
        in.read(buffer.data(), to_read); // data points to a writable zone of at least to_read bytes.
        // Bytes read (less near EOF).
        std::streamsize bytes_read = in.gcount();
        if (bytes_read == 0) break;
        if (!in) break;
        buffer.resize(bytes_read); // Trim to actual read.
        // std::cerr << "bytes_read=" << bytes_read << ", buffer.size()=" << buffer.size() << std::endl;
        // Backtrack if mid-word at block end.
        if (bytes_read == to_read && current_pos + bytes_read < end && !std::isspace(static_cast<unsigned char>(buffer.back()))) {
            size_t last_space = buffer.find_last_of(" \t\n\r\f\v");
            if (last_space != std::string::npos) {
                buffer.resize(last_space + 1);
                in.seekg(current_pos + buffer.size()); // Seek after space.
            } else {
                // std::cerr << "Debug: current_pos=" << current_pos << ", buffer.size()=" << buffer.size() << ", buffer.back()=" << (int)(unsigned char)buffer.back() << " '" << buffer.back() << "'" << std::endl;
                // std::cerr << "No space in buffer. First 100: " << buffer.substr(0,100) << std::endl;
                // std::cerr << "Last 100: " << buffer.substr(buffer.size()-100) << std::endl;
                assert(false && "Unexpected long word; check data/partition");
            }
        }
        std::stringstream ss(buffer);
        std::string word;
        while (ss >> word) {
            word = pr::cleanWord(word);
            if (!word.empty()) {
                onWordEncountered(word);
            }
        }
        current_pos += buffer.size(); // Promotes size_t to streamoff (may warn, safe).
    }
}