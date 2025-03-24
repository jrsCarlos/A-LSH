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

    string text = loadDocument(docsDir + "/" + docsNames[docIdx].string());
    vector<string> words = readWords(text);
 
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