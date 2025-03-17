#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_set>

#include "preprocessing.hh"

using namespace std;

void generate_shingles(unordered_set<string>& shingles, const string& text, int k) {
    size_t length = text.size();
    for (size_t i = 0; i <= length - k; ++i) {
        shingles.insert(text.substr(i, k));
    }
}

double jaccard_similarity(const unordered_set<string>& set1, const unordered_set<string>& set2) {
    int intersection_size = 0;
    int union_size = set1.size();

    for (const auto& shingle : set2) {
        if (set1.find(shingle) != set1.end()) {
            intersection_size++;
        } 
        else union_size++;
    }

    return (double)intersection_size / union_size;
}

int main() {
    ifstream inputFile("./docs/Frankenstein.txt");
    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();

    string text = buffer.str();
    inputFile.close();



    string symbols = ".,;:¿?¡!\"'()_-[]{}—…‐‑‒–—―/\\«»‹›‘’“”";
    unordered_set<char> punctuationMarks(symbols.begin(), symbols.end());



    ofstream outputFile("clean.txt");
    if (!outputFile.is_open()) {
        cerr << "No se pudo crear el archivo de salida." << endl;
        return 1;
    }

    toLowercase(text);
    removeStopwords(text, "en");
    cleanText(text, punctuationMarks);

    outputFile << text << endl;
    outputFile.close();
}