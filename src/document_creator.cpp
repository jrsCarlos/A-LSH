#include <random>
#include <algorithm>
#include "utils.hpp"
#include "preprocessing.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;

// Separa el contenido del documento en palabras
vector<string> readWords(string& text) {
    // Normalizamos el texto
    toLowercase(text);
    removeStopwords(text, getLanguage(text));
    cleanText(text);

    stringstream ss(text);
    vector<string> words;
    string word;

    // Leemos las palabras una por una
    while (ss >> word) words.push_back(word);
    return words;
}

// Genera un documento con las palabras permutadas
string generatePermutedDocument(const vector<string>& words, int num) {
    vector<string> permutedWords = words;
    random_device seed;
    mt19937 randomNumGenerator(seed());

    // Permutamos aleatoriamente las palabras
    shuffle(permutedWords.begin(), permutedWords.end(), randomNumGenerator);

    // Juntamos todas las palabras
    string permutedText = "";
    int numWords = permutedWords.size();
    for (int i = 0; i < numWords; ++i) {
        if (i == numWords - 1) permutedText += permutedWords[i];
        else permutedText += permutedWords[i] + " ";
    }

    return permutedText;
}

// Function to generate k-shingles from words
unordered_set<string> generateKShingles(const vector<string>& words, int k) {
    unordered_set<string> shingles;

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
void generateVirtualDocument(const unordered_set<string>& baseShingles, int num) {
    vector<string> shingles(baseShingles.begin(), baseShingles.end());

    // Shuffle the shingles randomly
    random_device rd;
    mt19937 randomNumGenerator(rd());
    shuffle(shingles.begin(), shingles.end(), randomNumGenerator);

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
    string docsDir = "./docs";
    string expDir = "./exp-docs";
    vector<fs::path> docsNames = getDocs(docsDir);

    cout << "Inserta el indice del documento base: " << endl;
    int numBaseDocs = docsNames.size();
    for (int i = 0; i < numBaseDocs; ++i) {
        cout << i << ". " << docsNames[i].stem() << endl;
    }
    int docIdx;
    cin >> docIdx;

    cout << "Introduce el numero de documentos a generar: " << endl;
    int numDocuments;
    cin >> numDocuments;
    if (numDocuments < 20) {
        cerr << "Error: El numero de documentos virtuales debe ser D >= 20." << endl;
        return 1;
    }

    cout << "Selecciona que bloque de documentos generar:" << endl;
    cout << "1. Permutar Documento Base" << endl;
    cout << "2. K-Shingles" << endl; 
    int choice;
    cin >> choice;

    string text = loadDocument(docsDir + "/" + docsNames[docIdx].string());
    vector<string> words = readWords(text);
 
    if(choice == 1){
        if (words.size() < 50) {
            cerr << "Error: El documento base debe tener al menos 50 palabras" << endl;
            return 1;
        }

        for (int i = 0; i < numDocuments; i++) {
            string permutedText = generatePermutedDocument(words, i + 1);
            string name = expDir + '/' + to_string(i+1) + "-" + docsNames[docIdx].string();
            saveDocument(permutedText, name);
        }
    }
    else { //k
        int k;
        cout << "Enter the k-shingle length (k): ";
        cin >> k;

        if (words.size() < 100) {
            cerr << "Error: The base document must contain at least 100 words." << endl;
            return 1;
        }

        unordered_set<string> baseShingles = generateKShingles(words, k);

        for (int i = 0; i < numDocuments; ++i) {
            generateVirtualDocument(baseShingles, i + 1);
        }
    }
  
    cout << "Virtual documents generated successfully!" << endl;
    return 0;
}