#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>
#include <vector>

#include "HT.h"

bool val_sort_decr(const std::pair<std::string, int> &a,
                   const std::pair<std::string, int> &b) {
  return (a.second > b.second);
}

int main() {
  using namespace std;
  using namespace std::chrono;

  ifstream input = ifstream("./WarAndPeace.txt");

  auto start = steady_clock::now();
  cout << "Parsing War and Peace" << endl;

  size_t nombre_lu = 0;
  // prochain mot lu
  string word;
  // une regex qui reconnait les caractères anormaux (négation des lettres)
  regex re(R"([^a-zA-Z])");
  pr::HashTable<string, int> ht(500);

  while (input >> word) {
    // élimine la ponctuation et les caractères spéciaux
    word = regex_replace(word, re, "");
    // passe en lowercase
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    int *n = ht.get(word);
    if (n == nullptr)
      ht.put(word, 0);
    else {
      ht.put(word, *n + 1);
    }

    // word est maintenant "tout propre"
    // if (nombre_lu % 100 == 0)
    //   // on affiche un mot "propre" sur 100
    //   cout << nombre_lu << ": " << word << endl;
    nombre_lu++;
  }
  input.close();

  cout << "Finished Parsing War and Peace" << endl;

  auto end = steady_clock::now();

  cout << "Parsing took " << duration_cast<milliseconds>(end - start).count()
       << "ms.\n";

  cout << "Found a total of " << nombre_lu << " words.\n";

  int *t = ht.get("toto");
  int toto = t == nullptr ? 0 : *t;

  cout << "Found a total of " << *ht.get("war") << " occurences of 'war'.\n";
  cout << "Found a total of " << *ht.get("peace")
       << " occurences of 'peace'.\n";
  cout << "Found a total of " << toto << " occurences of 'toto'.\n";

  vector<pair<string, int>> vec = ht.get_vec();

  sort(vec.begin(), vec.end(), val_sort_decr);

  std::cout << "10 most frequent words are:\n";
  for (auto i = 0; i < 10; i++) {
    std::cout << vec[i].first << "\n";
  }
    std::cout << std::endl;

  return 0;
}
