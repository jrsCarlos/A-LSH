#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Function to read the base document and extract words
vector<string> readWords(const string& filename) {
    ifstream file(filename); //read the document

    if (!file) {  // check if the file opened successfully
        cerr << "Error: Unable to open the file " << filename << endl;
        exit(1);
    }

    vector<string> words;   //vector with the words
    string word;

    while (file >> word) {
        words.push_back(word);
    }

    file.close();
    return words;
}

// Function to generate a virtual document with a random permutation
void generatePermutedDocument(const vector<string>& words, int numWords, int num) {
    vector<string> permutedWords = words;       //vector with permuted words
    random_device rd;
    mt19937 g(rd()); //random number generator (mt19937 is the fastest)
    shuffle(permutedWords.begin(), permutedWords.end(), g);

    ofstream file("document_" + to_string(num) + ".txt"); //generate the document
    if (!file) {
        cerr << "Error: Unable to open the file for writing." << endl;
        exit(1);
    }

    for (int i = 0; i < numWords; ++i) {
        file << permutedWords[i] << " ";
    }
    file.close();
}

int main() {
    string filename;
    int numVirtualDocs;
    int numWords;

    cout << "Enter the name of the base document file: ";
    cin >> filename;
    cout << "Enter the number of words in the new document: ";
    cin >> numWords;
    cout << "Enter the number of documents to generate: ";
    cin >> numVirtualDocs;

    // Read the base document
    vector<string> words = readWords(filename);
    if (words.size() < 50) {
        cerr << "Error: The base document must contain more than 50 words." << endl;
        return 1;
    }
    if (numWords > words.size()) {
        cerr << numWords << " " << words.size() << "Error: The base document has less words than the number you asked." << endl;
        return 1;
    }

    // Generate and save virtual documents with permutations
    for (int i = 0; i < numVirtualDocs; i++) {
        generatePermutedDocument(words, numWords, i + 1);
    }

    cout << "Virtual documents generated successfully!" << endl;
    return 0;
}