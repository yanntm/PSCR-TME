#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <ios>
#include <stdexcept>
#include "HashMap.h"
#include "FileUtils.h"

using namespace std;

int main(int argc, char **argv)
{
        using namespace std::chrono;

        // Arguments : fichier, mode, nombre de partitions (puis de threads).
        string filename = "../WarAndPeace.txt";
        string mode = "freqstd";
        int num_threads = 4;
        if (argc > 1)
                filename = argv[1];
        if (argc > 2)
                mode = argv[2];

        if (argc > 3) {
                try {
                        size_t consumed = 0;
                        num_threads = stoi(argv[3], &consumed);
                        if (argv[3][consumed] != '\0' || num_threads <= 0)
                                throw invalid_argument("num_threads");
                } catch (const exception&) {
                        cerr << "Le nombre de partitions doit etre un entier strictement positif." << endl;
                        return 2;
                }
        }

        // Vérification du fichier et calcul de sa taille.
        ifstream check(filename, std::ios::binary);
        if (!check.is_open())
        {
                cerr << "Could not open '" << filename << "'. Please provide a readable text file as the first argument." << endl;
                cerr << "Usage: " << (argc > 0 ? argv[0] : "countword") << " [path/to/textfile] [mode] [num_threads]" << endl;
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

        } else if (mode == "partition") {
                size_t total_words = 0;
                size_t unique_words = 0;
                std::unordered_map<std::string, int> um;
                auto parts = pr::partition(filename, file_size, num_threads);
                for (size_t i = 0; i < parts.size() - 1; ++i) {
                        pr::processRange(filename, parts[i], parts[i+1], [&](const std::string& word) {
                                total_words++;
                                um[word]++;
                        });
                }
                unique_words = um.size();
                pairs.reserve(unique_words);
                for (const auto& p : um) pairs.emplace_back(p);
                pr::printResults(total_words, unique_words, pairs, mode + ".freq");

        } else {
                cerr << "Unknown mode '" << mode << "'. Supported modes: freqstd, freqstdf, partition, freq" << endl;
                return 1;
        }

        // Temps total du traitement, tri et écriture des résultats compris.
        auto end = steady_clock::now();
        cout << "Total runtime (wall clock) : " << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

        return 0;
}
