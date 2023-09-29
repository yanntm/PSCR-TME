#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("./WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)

	//Question 2
	size_t nb_mots_differents = 0;
	std::vector<string> vect;
	
	//Question 3
	std::vector<pair<string,int>> vect_pair;

	regex re( R"([^a-zA-Z])");
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		
		//Question 2
		//On cherche s'il existe un mot différent
		bool found = false;
		for (int i=0; i<vect.size(); i++){
			if (!word.compare(vect[i])){
				found = true;
				break;
			}
		}

		//S'il n'en existe pas, on l'ajoute
		if (!found){
			vect.push_back(word);
			nb_mots_differents++;
		}

		//Question 3
		bool found2 = false;
		for (int i=0; i<vect_pair.size(); i++){
			if (!word.compare(vect_pair[i].first)){
				vect_pair[i].second++;
				found2 =true;
				break;
			}
		}

		if (!found2){

			vect_pair.push_back(pair<string,int>(word,1));
		}

		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
	cout << "Found a total of " << nb_mots_differents << " different words." << endl;

	bool found_war = false, found_peace = false, found_toto = false;
	for (int i=0; i<vect_pair.size(); i++){
		if (!(new string("war"))->compare(vect_pair[i].first)){
			cout << "Found a total of " << vect_pair[i].second << " war." << endl;
			found_war = true;
		}
		if (!(new string("peace"))->compare(vect_pair[i].first)){
			cout << "Found a total of " << vect_pair[i].second << " peace." << endl;
			found_peace = true;
		}
		if (!(new string("toto"))->compare(vect_pair[i].first)){
			cout << "Found a total of " << vect_pair[i].second << " toto." << endl;
			found_toto = true;
		}
	}

	if (!found_war) {
		cout << "Found a total of 0 war." << endl;
	}
	if (!found_peace) {
		cout << "Found a total of 0 peace." << endl;
	}
	if (!found_toto) {
		cout << "Found a total of 0 toto." << endl;
	}

    return 0;
}

