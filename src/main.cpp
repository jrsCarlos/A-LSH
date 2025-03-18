#include "hashing_tools.hpp"
#include "preprocessing.hpp"

using namespace std;
//Prueba
int main() {
    ifstream inputFile("./docs/Prova.txt");

    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();

    string text = buffer.str();
    inputFile.close();

    ///////////////////////////////////////////////////////////////////////////

    // Preprocesamiento del texto
    toLowercase(text);
    removeStopwords(text, "en");
    cleanText(text);

    unordered_set<string> shingles;
    generateShingles(shingles, text, 3);
    vector<uint64_t> minhashSignature = getMinhashSignature(shingles, 10);
    for (const auto& hash : minhashSignature) {
        cout << hash << endl;
    }

    ///////////////////////////////////////////////////////////////////////////

    ofstream outputFile("clean-docs/ProvaClean.txt");
    if (!outputFile.is_open()) {
        cerr << "No se pudo crear el archivo de salida." << endl;
        return 1;
    }

    outputFile << text << endl;
    outputFile.close();
}