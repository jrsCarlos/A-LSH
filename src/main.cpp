#include "utils.hpp"
#include "hashing_tools.hpp"
namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};


int main() {

    //Get time stamp begining
    auto Tstart = high_resolution_clock::now();

    // Leemos todos los documentos
    string docsDir = "./exp-docs";
    vector<fs::path> docs = getDocs(docsDir);
    int numOfDocs = docs.size();

    auto Tread = high_resolution_clock::now();
    /////////////////////////////// GENERACION DE SHINGLES ///////////////////////////////

    int K = 5;
    vector<ShingleSet> docsShingles(numOfDocs);
    //Vector with size of documents and shingles
    vector<int> sizeOfDocs(numOfDocs);
    vector<int> sizeOfDocs2(numOfDocs);
    vector<int> sizeOfDocs3(numOfDocs);

    for (int i = 0; i < numOfDocs; ++i) {
        string docPath = docsDir + "/" + docs[i].string();
        string docText = loadDocument(docPath);
        generateShingles(docsShingles[i], docText, K);
        //Save size of document
        sizeOfDocs[i] = docText.size();
        sizeOfDocs2[i] = docsShingles[i].size();
    }
    auto Tshingles = high_resolution_clock::now();
    /////////////////////////////// GENERACION DE MINHASHES //////////////////////////////

    int numHashes = 200;
    vector<vector<uint64_t>> docsSignatures(numOfDocs);

    //Vector with size of documents after shingles


    for (int i = 0; i < numOfDocs; ++i) {
        docsSignatures[i] = getMinhashSignature(docsShingles[i], numHashes);
        cout << "Signature doc " << i << ": ";
        for (auto val : docsSignatures[i]) cout << val << " ";
        cout << endl;
        //Save size of documents minHash
        sizeOfDocs3[i] = docsSignatures[i].size();
    }
    auto TminHash = high_resolution_clock::now();

    //////////////////////////////////////// LSH /////////////////////////////////////////

    int bands = 50;
    int rows = 4;
    if (bands * rows != numHashes) {
        cout << "Error: bands * rows != numHashes" << endl;
        return 1;
    }
    vector<pair<int, int>> candidatePairs = LSH(docsSignatures, bands, rows);

    //////////////////////////////////// ESTADISTICAS ////////////////////////////////////

    double recall = 0.0;
    double precision = 0.0;
    int falsePositives = 0;
    int falseNegatives = 0;
    double truePositives = 0;
    double simThreshold = 0.4;

    /// CALCULO DE LOS PARES VERDADEROS ///
    unordered_set<pair<int,int>, pair_hash> trueSimilarPairs;
    for (int i = 0; i < numOfDocs; ++i) {
        for (int j = i + 1; j < numOfDocs; ++j) {
            double sim = shinglesJaccardSimilarity(docsShingles[i], docsShingles[j]);
            cout << "Sim between " << i << " and " << j << ": " << sim << endl;
            if (sim >= simThreshold) {
                trueSimilarPairs.insert({i,j});
            }
        }
    }

    unordered_set<pair<int, int>, pair_hash> candidateSet(
        candidatePairs.begin(), candidatePairs.end()
    );
    
    // True Positives y False Positives
    for (const auto& pair : candidatePairs) {
        if (trueSimilarPairs.count(pair)) ++truePositives;
        else ++falsePositives;
    }
    
    // False Negatives: Pares verdaderos no en candidatos
    for (const auto& truePair : trueSimilarPairs) {
        if (!candidateSet.count(truePair)) ++falseNegatives;
    }
    
    // Cálculo de recall y precisión
    if (truePositives + falseNegatives > 0) {
        recall = truePositives / (truePositives + falseNegatives);
    }
    if (truePositives + falsePositives > 0) {
        precision = truePositives / (truePositives + falsePositives);
    }

    //Final Timestamp
    auto Tfinal = high_resolution_clock::now();

    
    
    ///////////////////////////////// CALCULO DE TIEMPOS /////////////////////////////////

    //Duration to read
    auto Dread = duration_cast<microseconds>(Tread - Tstart);

    //Duration to generate shingles
    auto Dshingles = duration_cast<microseconds>(Tshingles - Tread);

    //Duration to minHash
    auto DminHash = duration_cast<microseconds>(TminHash - Tshingles);

    //Duration to finish
    auto Dtotal = duration_cast<microseconds>(Tfinal - Tstart);

    ofstream dataFile;
    dataFile.open("dataFile.txt");


    //Data
    dataFile << endl << "-------------Data-------------" << endl;
    dataFile << "Number of Files: " << numOfDocs << " files" << endl;
    for (int i = 0; i < numOfDocs; ++i) {
        //Print sizes of documents in each fase
        dataFile << "--- Sizes of Document " << i << " ---" << endl;
        dataFile << "Cleaned: " << sizeOfDocs[i] << " bytes   ";
        dataFile << "Shingles: " << sizeOfDocs2[i] * K << " bytes   ";
        dataFile << "MinHash: " << sizeOfDocs3[i] * sizeof(uint64_t) << " bytes" << endl;
    }

    /*cout << "Text size before cleanup: " << textSizeBC << " bytes/characters" << endl;
    cout << "Text size after cleanup: " << textSizeAC << " bytes/characters" << endl;
    cout << "We removed: " << textSizeBC - textSizeAC << " bytes/characters" << endl;
    cout << "Num of Shingles: " << shinglesNum << " shingles" << endl;
    cout << "Size of Shingles: " << shinglesSize << " bytes" << endl;
    cout << "Num of MinHash: " << minHashNum << " shingles" << endl;
    cout << "Size of MinHash: " << minHashSize << " bytes" << endl;*/

    //Time
    dataFile << endl << "-------------Time-------------" << endl;
    dataFile << "Time to read clean files: " << Dread.count() << " ms" << endl;
    dataFile << "Time to generate shingles: " << Dshingles.count() << " ms" << endl;
    dataFile << "Time to generate minHash: " << DminHash.count() << " ms" << endl;
    dataFile << "Total time processing: " << Dtotal.count() << " ms" << endl;

    //Statistics
    dataFile << endl << "-------------Statistics-------------" << endl;
    dataFile << "True Positives: " << truePositives << endl;
    dataFile << "False Positives: " << falsePositives << endl;
    dataFile << "False Negatives: " << falseNegatives << endl;
    dataFile << "Precision: " << precision << endl;
    dataFile << "Recall: " << recall << endl;

}