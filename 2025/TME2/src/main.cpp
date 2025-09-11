#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

int main(int argc, char** argv) {
	using namespace std;
	using namespace std::chrono;

	// Allow filename as optional first argument, default to project-root/WarAndPeace.txt
	string filename = "../WarAndPeace.txt";
	if (argc > 1) filename = argv[1];

	ifstream input(filename);
	if (!input.is_open()) {
		cerr << "Could not open '" << filename << "'. Please provide a readable text file as the first argument." << endl;
		cerr << "Usage: " << (argc>0?argv[0]:"TME2") << " [path/to/textfile]" << endl;
		return 2;
	}

	auto start = steady_clock::now();
	cout << "Parsing " << filename << endl;

	size_t nombre_lu = 0;
	string word;
	regex re(R"([^a-zA-Z])");
	while (input >> word) {
		word = regex_replace(word, re, "");
		transform(word.begin(), word.end(), word.begin(), ::tolower);

		if (nombre_lu % 100 == 0)
			cout << nombre_lu << ": " << word << endl;
		nombre_lu++;
	}
	input.close();

	cout << "Finished parsing." << endl;

	auto end = steady_clock::now();
	cout << "Parsing took " << duration_cast<milliseconds>(end - start).count() << "ms.\n";
	cout << "Found a total of " << nombre_lu << " words." << endl;

	return 0;
}


