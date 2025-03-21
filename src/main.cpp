#include "hashing_tools.hpp"
#include "preprocessing.hpp"
#include "document_generator.hpp"
#include <filesystem>

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std;

string loadDocument(const string& docPath) {
    ifstream docContent(docPath);
    stringstream buffer;
    buffer << docContent.rdbuf();
    return buffer.str();
}

void saveDocument(const string& docText, const string& docPath) {
    ofstream docContent(docPath);
    docContent << docText;
    docContent.close();
}

int main() {
    //////////////////////////////// PREPROCESAMIENTO ////////////////////////////////

    // Leemos todos los documentos del directorio docs
    string docsDir = "./docs";
    string expDir = "./exp-docs";
    string cleanDocsDir = "./clean-docs";
    
    int numDocs = 0;
    vector<fs::path> docsNames;
    for (const auto& doc : fs::directory_iterator(docsDir)) {
        if (doc.is_regular_file()) {
            docsNames.push_back(doc.path().filename());
            ++numDocs;
        }
    }
    
    for (int i = 0; i < numDocs; ++i) {
        // Extraemos el texto de cada documento
        string docText = loadDocument(docsDir + "/" + docsNames[i].string());

        // Pasamos todo el texto a minimusculas para detectar todas las stopwords
        toLowercase(docText);

        // Limpiamos el resto del texto
        removeStopwords(docText, getLanguage(docText));
        cleanText(docText);
        saveDocument(docText, cleanDocsDir + "/" + docsNames[i].string());
    }

    /////////////////// GENERACION DE LOS CONJUNTOS DE DOCUMENTOS ///////////////////

    // BLOQUE 1
    int D = 20;
    for (int i = 0; i < numDocs; ++i) {
        string docText = loadDocument(cleanDocsDir + "/" + docsNames[i].string());

        for (int j = 0; j < D; ++j) {
            string permutedText = generatePermutedText(docText);
            string newName = "/" + to_string(j+1) + "_Permuted_" + docsNames[i].string();
            saveDocument(permutedText, expDir + newName);
        }
    }
    /*
    //Get time stamp begining
    auto Tstart = high_resolution_clock::now();

    ifstream inputFile("./docs/Prova.txt");

    stringstream buffer;
    buffer << inputFile.rdbuf();

    string text = buffer.str();
    inputFile.close();

    //Timestamp reading file
    auto Tread = high_resolution_clock::now();
    //Text size before cleanup
    int textSizeBC = text.size();

    ///////////////////////////////////////////////////////////////////////////
    
    // Preprocesamiento del texto
    lowerStrangeLetters(text);
    toLowercase(text);
    removeStopwords(text, "en");
    cleanText(text);

    //Timestamp finish cleaning text
    auto Tclean = high_resolution_clock::now();
    //Text size after cleanup
    int textSizeAC = text.size();

    ShingleSet shingles;

    generateShingles(shingles, text, 3);

    //Timestamp finish generating shingles
    auto Tshingles = high_resolution_clock::now();
    //shingle size
    int shinglesNum = shingles.size();
    int shinglesSize = shinglesNum * 3;

    vector<uint64_t> minhashSignature = getMinhashSignature(shingles, 10);

    //Timestamp finish generating minHash
    auto TminHash = high_resolution_clock::now();
    //Size of minhash
    int minHashNum = minhashSignature.size();
    int minHashSize = minHashNum * sizeof(uint64_t);

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

    //Final Timestamp
    auto Tfinal = high_resolution_clock::now();

    
    

    //time calculations
    
    //Duration to read
    auto Dread = duration_cast<microseconds>(Tread - Tstart);

    //Duration to clean
    auto Dclean = duration_cast<microseconds>(Tclean - Tread);

    //Duration to generate shingles
    auto Dshingles = duration_cast<microseconds>(Tshingles - Tclean);

    //Duration to minHash
    auto DminHash = duration_cast<microseconds>(TminHash - Tshingles);

    //Duration to finish
    auto Dtotal = duration_cast<microseconds>(Tfinal - Tstart);

    //print times
    cout << endl << "-------------Data-------------" << endl;
    cout << "Text size before cleanup: " << textSizeBC << " bytes/characters" << endl;
    cout << "Text size after cleanup: " << textSizeAC << " bytes/characters" << endl;
    cout << "We removed: " << textSizeBC - textSizeAC << " bytes/characters" << endl;
    cout << "Num of Shingles: " << shinglesNum << " shingles" << endl;
    cout << "Size of Shingles: " << shinglesSize << " bytes" << endl;
    cout << "Num of MinHash: " << minHashNum << " shingles" << endl;
    cout << "Size of MinHash: " << minHashSize << " bytes" << endl;

    cout << endl << "-------------Time-------------" << endl;
    cout << "Time to read files: " << Dread.count() << " ms" << endl;
    cout << "Time to clean: " << Dclean.count() << " ms" << endl;
    cout << "Time to generate shingles: " << Dshingles.count() << " ms" << endl;
    cout << "Time to generate minHash: " << DminHash.count() << " ms" << endl;
    cout << "Total time: " << Dtotal.count() << " ms" << endl;
    */
}
