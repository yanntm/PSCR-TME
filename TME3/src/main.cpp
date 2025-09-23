#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <ios>
#include "HashMap.h"
#include "FileUtils.h"

using namespace std;

int main(int argc, char **argv)
{
        using namespace std::chrono;

        // Allow filename as optional first argument, default to project-root/WarAndPeace.txt
        // Optional second argument is mode (e.g. "freqstd" or "freq").
        // Optional third argument is num_threads (default 4).
        string filename = "../WarAndPeace.txt";
        string mode = "freqstd";
        int num_threads=4;
        if (argc > 1)
                filename = argv[1];
        if (argc > 2)
                mode = argv[2];

        // Check if file is readable
        ifstream check(filename, std::ios::binary);
        if (!check.is_open())
        {
                cerr << "Could not open '" << filename << "'. Please provide a readable text file as the first argument." << endl;
                cerr << "Usage: " << (argc > 0 ? argv[0] : "TME3") << " [path/to/textfile] [mode] [num threads]" << endl;
                return 2;
        }
        check.seekg(0, std::ios::end);
        std::streamoff file_size = check.tellg();
        check.close();

        cout << "Preparing to parse " << filename << " (mode=" << mode << " N=" << num_threads << "), containing " << file_size << " bytes" << endl;

        auto start = steady_clock::now();

        std::vector<std::pair<std::string, int>> pairs;

        if (mode == "freqstd") {
                ifstream input(filename, std::ios::binary);
                size_t total_words = 0;
                size_t unique_words = 0;
                std::unordered_map<std::string, int> um;
                std::string word;
                while (input >> word) {
                        word = pr::cleanWord(word);
                        if (!word.empty()) {
                                total_words++;
                                ++um[word];
                        }
                }
                unique_words = um.size();
                pairs.reserve(unique_words);
                for (const auto& p : um) pairs.emplace_back(p);
                pr::printResults(total_words, unique_words, pairs, mode + ".freq");

        } else if (mode == "freqstdf") {
                size_t total_words = 0;
                size_t unique_words = 0;
                std::unordered_map<std::string, int> um;
                pr::processRange(filename, 0, file_size, [&](const std::string& word) {
                        total_words++;
                        um[word]++;
                });
                unique_words = um.size();
                pairs.reserve(unique_words);
                for (const auto& p : um) pairs.emplace_back(p);
                pr::printResults(total_words, unique_words, pairs, mode + ".freq");

        } else if (mode == "freq") {
                size_t total_words = 0;
                size_t unique_words = 0;
                HashMap<std::string, int> hm;
                pr::processRange(filename, 0, file_size, [&](const std::string& word) {
                        total_words++;
                        hm.incrementFrequency(word);
                });
                pairs = hm.toKeyValuePairs();
                unique_words = pairs.size();
                pr::printResults(total_words, unique_words, pairs, mode + ".freq");

        } else {
                cerr << "Unknown mode '" << mode << "'. Supported modes: freqstd, freq, freqstdf" << endl;
                return 1;
        }

        // print a single total runtime for successful runs
        auto end = steady_clock::now();
        cout << "Total runtime (wall clock) : " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

        return 0;
}

