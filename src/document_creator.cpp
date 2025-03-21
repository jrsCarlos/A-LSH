#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>
#include <random>
#include <set>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

unordered_set<string> loadStopwords(const string& language) {
    string filePath = "deps/stopwords.json";
    ifstream jsonFile(filePath);
    if (!jsonFile) cerr << "No se pudo leer " << filePath << endl;

    json jsonStopwords;
    jsonFile >> jsonStopwords;

    unordered_set<string> stopwords;
    vector<string> vectorStopwords = jsonStopwords[language].get<vector<string>>();
    stopwords.insert(vectorStopwords.begin(), vectorStopwords.end());

    return stopwords;
}

string getLanguage(const string& text) {
    // Contamos las ocurrencias de las stopwords de cada idioma
    vector<string> languages = {"en", "ca", "es"};
    unordered_set<string> stopwords;

    int languageIdx = 0;
    int maxCount = -1;

    int numLanguages = languages.size();
    for (int i = 0; i < numLanguages; i++) {
        stopwords = loadStopwords(languages[i]);
        int count = 0;

        stringstream ss(text);
        string word;

        while (ss >> word) {
            if (stopwords.count(word)) ++count;
        }

        if (count > maxCount) {
            maxCount = count;
            languageIdx = i;
        }
    }
    return languages[languageIdx];
}

void cleanText(string& text) {
    string symbols = ".,;:¿?¡!\"'()_-[]{}—…‐‑‒–—―/\\«»‹›‘’“”";
    unordered_set<char> punctuationMarks(symbols.begin(), symbols.end());

    // Indice que indica donde se va a escribir en el texto
    size_t writeIndex = 0;
    bool space = false;

    for (size_t i = 0; i < text.size(); ++i) {
        // Sustituimos tabulaciones (\t) y saltos de linea (\n) por espacios
        if (text[i] == '\t' || text[i] == '\n') {
            if (!space) {
                text[writeIndex++] = ' ';
                space = true;
            }
            continue;
        }

        // Eliminar signos de puntuacion
        if (!punctuationMarks.count(text[i])) {
            // Comprimimos espacios consecutivos
            if (text[i] == ' ') {
                // Si no hay un espacio previo, agregamos uno
                if (!space) {
                    text[writeIndex++] = text[i];
                    space = true;
                }
            }
            else {
                // Agregamos el caracter
                text[writeIndex++] = text[i];
                space = false;
            }
        }
    }
    
    // Eliminamos los caracteres sobrantes
    text.erase(writeIndex);
}

void toLowercase(string& text) {
    map<char,char> specialLetters = {
        {'À', 'a'}, {'Á', 'a'}, {'Â', 'a'}, {'Ä', 'a'},
        {'È', 'e'}, {'É', 'e'}, {'Ê', 'e'}, {'Ë', 'e'},
        {'Ì', 'i'}, {'Í', 'i'}, {'Î', 'i'}, {'Ï', 'i'},
        {'Ò', 'o'}, {'Ó', 'o'}, {'Ô', 'o'}, {'Ö', 'o'},
        {'Ù', 'u'}, {'Ú', 'u'}, {'Û', 'u'}, {'Ü', 'u'},
        {'Ç', 'ç'}, {'Ñ', 'ñ'}
    };

    for (char& letter : text) {
        if (specialLetters.count(letter)) letter = specialLetters[letter];
        letter = tolower(letter);
    }
}

void removeStopwords(string& text, const string& language) {
    unordered_set<string> stopwords = loadStopwords(language);
    
    stringstream ss(text);
    string word;
    string noStopwordsText;

    while (ss >> word) {
        // Si no es una stopword, la agregamos al texto
        if (stopwords.find(word) == stopwords.end()) {
            // Agregamos espacios para que no se junten las palabras
            if (!noStopwordsText.empty()) noStopwordsText += " ";
            noStopwordsText += word;
        }
    }

    text = noStopwordsText;
}
// Function to read the base document and extract words
vector<string> readWords(const string& filename) {
    ifstream file(filename); //read the document

    if (!file) {  // check if the file opened successfully
        cerr << "Error: Unable to open the file " << filename << endl;
        exit(1);
    }


    stringstream buffer;
    buffer << file.rdbuf();
    string text = buffer.str();
    file.close();

    toLowercase(text);
    cleanText(text);
    string language = getLanguage(text);
    removeStopwords(text, language);

    vector<string> words;
    stringstream ss(text);
    string word;
    while (ss >> word) {
        words.push_back(word);
    }

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
    int numWords;
    char choice;
    int numDocuments;
    int k;

    cout << "Enter the name of the base document file: ";
    cin >> filename;
    cout << "Enter the number of documents to generate: ";
    cin >> numDocuments;
    cout << "Permutate Document(P) or K-shingles(K): ";
    cin >> choice;
    if(choice == 'P'){
        cout << "Enter the number of words in the new document: ";
        cin >> numWords;
    } else if( choice == 'K'){
        cout << "Enter the k-shingle length (k): ";
        cin >> k;
    }
    else { cout << "Invalid option" << endl;
    }

    // Read the base document
    vector<string> words = readWords(filename);
 
    if(choice == 'P'){
        if (words.size() < 50) {
            cerr << "Error: The base document must contain at least 50 words." << endl;
            return 1;
        }
        if (numWords > words.size()) {
            cerr << numWords << " " << words.size() << "Error: The base document has less words than the number you asked." << endl;
            return 1;
        }

          // Generate and save virtual documents with permutations
        for (int i = 0; i < numDocuments; i++) {
            generatePermutedDocument(words, numWords, i + 1);
        }

    }
    else{ //k
        if (words.size() < 100) {
            cerr << "Error: The base document must contain at least 100 words." << endl;
            return 1;
        }
        if (numDocuments < 20) {
            cerr << "Error: The number of virtual documents must be D >= 20." << endl;
            return 1;
        }
        set<string> baseShingles = generateKShingles(words, k);

        // Generate and store virtual documents with random k-shingles
        for (int i = 0; i < numDocuments; ++i) {
            generateVirtualDocument(baseShingles, i + 1);
        }
    }
  
    cout << "Virtual documents generated successfully!" << endl;
    return 0;
}