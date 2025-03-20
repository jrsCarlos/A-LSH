#include "hashing_tools.hpp"
#include "preprocessing.hpp"
using namespace std::chrono;
using namespace std;

int main() {

    //Get time stamp begining
    auto Tstart = high_resolution_clock::now();

    ifstream inputFile("./docs/Prova.txt");

    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

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

    vector<uint64_t> minhashSignature = getMinhashSignature(shingles, 10);

    //Timestamp finish generating minHash
    auto TminHash = high_resolution_clock::now();

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
    cout << "Text size before cleanup: " << textSizeBC << " characters" << endl;
    cout << "Text size after cleanup: " << textSizeAC << " characters" << endl;
    cout << "We removed: " << textSizeBC - textSizeAC << " characters" << endl;
    cout << "Num of Shingles: " << shinglesNum << " shingles" << endl;

    cout << endl << "-------------Time-------------" << endl;
    cout << "Time to read files: " << Dread.count() << " ms" << endl;
    cout << "Time to clean: " << Dclean.count() << " ms" << endl;
    cout << "Time to generate shingles: " << Dshingles.count() << " ms" << endl;
    cout << "Time to generate minHash: " << DminHash.count() << " ms" << endl;
    cout << "Total time: " << Dtotal.count() << " ms" << endl;

}
