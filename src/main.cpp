#include "utils.hpp"
#include "hashing_tools.hpp"

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std;

int main() {

    //Get time stamp begining
    auto Tstart = high_resolution_clock::now();

    // Leemos todos los documentos
    string docsDir = "./exp-docs";
    vector<fs::path> docs = getDocs(docsDir);


    /////////////////////////////// GENERACION DE SHINGLES ///////////////////////////////

    int K = 5;
    vector<ShingleSet> docsShingles(docs.size());

    for (int i = 0; i < docs.size(); ++i) {
        string docPath = docsDir + "/" + docs[i].string();
        string docText = loadDocument(docPath);
        generateShingles(docsShingles[i], docText, K);
    }

    /////////////////////////////// GENERACION DE MINHASHES //////////////////////////////

    int numHashes = 10;
    vector<vector<uint64_t>> docsSignatures(docs.size());

    for (int i = 0; i < docs.size(); ++i) {
        docsSignatures[i] = getMinhashSignature(docsShingles[i], numHashes);
    }

    //////////////////////////////////////// LSH /////////////////////////////////////////

    int bands = 5;
    int rows = 2;
    if (bands * rows != numHashes) {
        cout << "Error: bands * rows != numHashes" << endl;
        return 1;
    }
    vector<pair<int, int>> candidatePairs = LSH(docsSignatures, bands, rows);
    cout << "Candidate pairs: " << candidatePairs.size() << endl;
    for (const auto& pair : candidatePairs) {
        cout << pair.first << " " << pair.second << endl;
    }

    //////////////////////////////////// ESTADISTICAS ////////////////////////////////////

    double recall = 0.0;
    double precision = 0.0;
    int truePositives =  0;
    int falsePositives = 0;
    int falseNegatives = 0;
    double simThreshold = 0.8;

    /// CALCULO DE LOS PARES VERDADEROS ///
    unordered_set<string> trueSimilarPairs;
    for (size_t i = 0; i < docsShingles.size(); ++i) {
        for (size_t j = i+1; j < docsShingles.size(); ++j) {
            double sim = shinglesJaccardSimilarity(docsShingles[i], docsShingles[j]);
            if (sim >= simThreshold) {
                trueSimilarPairs.insert(to_string(i) + "-" + to_string(j));
            }
        }
    }

    /// CALCULO DE CANDIDATE PAIRS ///
    for (const auto& pair : candidatePairs) {
        string pair_str = to_string(pair.first) + "-" + to_string(pair.second);
        
        if (trueSimilarPairs.count(pair_str)) {
            truePositives++;
        }
        else ++falsePositives;
    }

    falseNegatives = trueSimilarPairs.size() - truePositives;

    /// CALCULO DE PRECISION Y RECALL ///
    if (!candidatePairs.empty()) {
        precision = static_cast<double>(truePositives) / candidatePairs.size();
    }
    
    if (!trueSimilarPairs.empty()) {
        recall = static_cast<double>(truePositives) / trueSimilarPairs.size();
    }


    cout << "True Positives: " << truePositives << endl;
    cout << "False Positives: " << falsePositives << endl;
    cout << "False Negatives: " << falseNegatives << endl;
    cout << "Precision: " << precision << endl;
    cout << "Recall: " << recall << endl;

    //Final Timestamp
    auto Tfinal = high_resolution_clock::now();

    
    
    ///////////////////////////////// CALCULO DE TIEMPOS /////////////////////////////////

    //Duration to finish
    auto Dtotal = duration_cast<microseconds>(Tfinal - Tstart);
    cout << "Total time: " << Dtotal.count() << " ms" << endl;


    /* OLD TIME CALC
        //Duration to read
    auto Dread = duration_cast<microseconds>(Tread - Tstart);

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
