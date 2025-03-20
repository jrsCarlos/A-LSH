#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <set>

using namespace std;

// Function to read the words from a file
vector<string> readWords(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Unable to open the file " << filename << endl;
        exit(1);
    }

    vector<string> words;
    string word;

    while (file >> word) {
        // Clean words (remove punctuation and convert to lowercase)
        word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.empty()) {
            words.push_back(word);
        }
    }

    file.close();
    return words;
}

// Function to generate k-shingles from words
set<string> generateKShingles(const vector<string>& words, int k) {
    set<string> shingles;

    // Iterate through the words and create k-shingles
    for (size_t i = 0; i <= words.size() - k; ++i) {
        string shingle = "";
        for (int j = 0; j < k; ++j) {
            shingle += words[i + j] + " ";
        }
        shingle.pop_back();  // Remove the extra space at the end
        shingles.insert(shingle);  // Insert the shingle into the set
    }

    return shingles;
}

// Function to generate a virtual document with random k-shingles
void generateVirtualDocument(const set<string>& baseShingles, int num) {
    vector<string> shingles(baseShingles.begin(), baseShingles.end());

    // Shuffle the shingles randomly
    random_device rd;
    mt19937 g(rd());
    shuffle(shingles.begin(), shingles.end(), g);

    // Select a random subset of shingles (50 shingles for example)
    int numShinglesToSelect = min(50, (int)shingles.size());

    // Write the shingles into the virtual document
    ofstream file("virtual_document_" + to_string(num) + ".txt");
    if (!file) {
        cerr << "Error: Unable to open the file for writing." << endl;
        exit(1);
    }

    for (int i = 0; i < numShinglesToSelect; ++i) {
        file << shingles[i] << "\n";
    }

    file.close();
}

int main() {
    string filename;
    int numDocuments;
    int k;

    // Prompt user for input
    cout << "Enter the base document filename: ";
    cin >> filename;
    cout << "Enter the number of virtual documents to generate (D >= 20): ";
    cin >> numDocuments;
    cout << "Enter the k-shingle length (k): ";
    cin >> k;

    // Validate the number of documents
    if (numDocuments < 20) {
        cerr << "Error: The number of virtual documents must be D >= 20." << endl;
        return 1;
    }

    // Read the base document
    vector<string> words = readWords(filename);
    if (words.size() < 100) {
        cerr << "Error: The base document must contain at least 100 words." << endl;
        return 1;
    }

    // Generate the k-shingles from the base document
    set<string> baseShingles = generateKShingles(words, k);

    // Generate and store virtual documents with random k-shingles
    for (int i = 0; i < numDocuments; ++i) {
        generateVirtualDocument(baseShingles, i + 1);
    }

    cout << "Virtual documents generated successfully!" << endl;
    return 0;
}
