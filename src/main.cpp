#include "utils.hpp"
#include "hashing_tools.hpp"
#include <cstdlib>
#include <ctime>
#include <numeric>
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

float average(vector<int> const& v){
    if (v.empty()) return 0;
    auto const count = static_cast<float>(v.size());
    return reduce(v.begin(), v.end()) / count;
}


int main() {
    srand(time(0));
    vector<int> kValues = {2, 4, 6, 8};
    vector<int> numHashesValues = {50, 100, 200, 300};
    vector<int> bandsValues = {10, 20, 50, 100};
    vector<int> rowsValues = {2, 3, 4, 5, 6};
    vector<double> simThresholdValues = {0.4/*, 0.6, 0.8*/};

    ofstream csvFile;
    csvFile.open("dataFile.csv");

    ofstream simsCsvFile;
    simsCsvFile.open("simsFile.csv");
    simsCsvFile << "Documet1,Document2,numHashes,ShinglesJaccard,MinHashJaccard" << endl;

    ofstream aproxCsvFile;
    aproxCsvFile.open("aproxFile.csv");
    aproxCsvFile << "Documet1,Document2,ShinglesJaccard,AproxSim" << endl;

    // Encabezado del CSV
    csvFile << "K,numHashes,numBandas,rows,Threshold,";
    csvFile << "NumOfDocs,";
    csvFile << "Bytes_CleaneDoc,Bytes_ShinglesDoc,Bytes_MinHashDoc,";
    csvFile << "Time_ReadCleanFiles,Time_GenerateShingles,Time_GenerateMinHash,Time_TotalProcessing,"; 
    csvFile << "TruePositives,FalsePositives,FalseNegatives,Precision,Recall" << endl;
    

    double aprox = 0.0;
    for (int ii = 0; ii < kValues.size(); ++ii) {
        for (int jj = 0; jj < numHashesValues.size(); ++jj) {
            for (int kk = 0; kk < bandsValues.size(); ++kk) {
                for (int ll = 0; ll < rowsValues.size(); ++ll) {
                    for (int mm = 0; mm < simThresholdValues.size(); ++mm) {

                        

                        int K = kValues[ii];
                        int numHashes = numHashesValues[jj];
                        int bands = bandsValues[kk];
                        int rows = rowsValues[ll];
                        double simThreshold = simThresholdValues[mm];

                        

                        if (bands * rows != numHashes) continue;
                        //Get time stamp begining
                        auto Tstart = high_resolution_clock::now();

                        // Leemos todos los documentos
                        string docsDir = "./exp-docs";
                        vector<fs::path> docs = getDocs(docsDir);
                        int numOfDocs = docs.size();

                        auto Tread = high_resolution_clock::now();
                        /////////////////////////////// GENERACION DE SHINGLES ///////////////////////////////

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

                        vector<vector<uint64_t>> docsSignatures(numOfDocs);

                        //Vector with size of documents after shingles

                        for (int i = 0; i < numOfDocs; ++i) {
                            docsSignatures[i] = getMinhashSignature(docsShingles[i], numHashes);
                            //cout << "Signature doc " << i << ": ";
                            //for (auto val : docsSignatures[i]) cout << val << " ";
                            //cout << endl;
                            //Save size of documents minHash

                            sizeOfDocs3[i] = docsSignatures[i].size();
                        }
                        auto TminHash = high_resolution_clock::now();

                        //////////////////////////////////////// LSH /////////////////////////////////////////

                        vector<pair<int, int>> candidatePairs = LSH(docsSignatures, bands, rows);

                        //////////////////////////////////// ESTADISTICAS ////////////////////////////////////

                        double recall = 0.0;
                        double precision = 0.0;
                        int falsePositives = 0;
                        int falseNegatives = 0;
                        double truePositives = 0;

                        for (int i = 0; i < numOfDocs; ++i) {
                            for (int j = i + 1; j < numOfDocs; ++j) {
                                ShingleSet s1 = obtenerSubconjuntoAleatorio(docsShingles[i], rand() % numOfDocs + 1);
                                ShingleSet s2 = obtenerSubconjuntoAleatorio(docsShingles[j], rand() % numOfDocs + 1);
                                

                                double siim = shinglesJaccardSimilarity(s1, s2);
                                double siim2 = similitudShingles(s1.size(), s2.size(), numOfDocs);
                                aproxCsvFile << docs[i].stem() << "," << docs[j].stem() << "," << siim << "," << siim2 << endl;
                            }
                        }

                        /// CALCULO DE LOS PARES VERDADEROS ///
                        unordered_set<pair<int,int>, pair_hash> trueSimilarPairs;
                        for (int i = 0; i < numOfDocs; ++i) {
                            for (int j = i + 1; j < numOfDocs; ++j) {
                                double sim = shinglesJaccardSimilarity(docsShingles[i], docsShingles[j]);
                                double sim2 = minhashJaccardSimilarity(docsSignatures[i], docsSignatures[j]);
                                //cout << "Sim between " << docs[i].stem() << " and " << docs[j].stem() << ": " << sim << endl;
                                simsCsvFile << docs[i].stem() << "," << docs[j].stem() << ","  << numHashes << "," << sim << "," << sim2 << endl;
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

                        /*ofstream dataFile;
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
                        dataFile << "Recall: " << recall << endl;*/

                        csvFile << K << "," << numHashes << "," << bands << "," << rows << "," << simThreshold << ",";
                        csvFile << numOfDocs << ",";
                        csvFile << average(sizeOfDocs) << "," 
                                << average(sizeOfDocs2) * K << ","
                                << average(sizeOfDocs3) * sizeof(uint64_t) << ",";
                        csvFile << Dread.count() << ","
                                << Dshingles.count() << ","
                                << DminHash.count() << ","
                                << Dtotal.count() << ",";
                        csvFile << truePositives << ","
                                << falsePositives << ","
                                << falseNegatives << ","
                                << precision << ","
                                << recall << endl;
                    }
                }
            }
        }
    }
}