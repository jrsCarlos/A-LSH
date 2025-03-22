#include "hashing_tools.hpp"

/////////////////////////////////////// SHINGLES ///////////////////////////////////////

void generateShingles(ShingleSet& shingles, const string& text, int k) {
    size_t length = text.size();
    for (size_t i = 0; i <= length - k; ++i) {
        shingles.insert(text.substr(i, k));
    }
}

double shinglesJaccardSimilarity(const ShingleSet& s1, const ShingleSet& s2) {
    double numIntersections = 0;
    int totalShingles = s1.size();

    for (const string& shingle : s2) {
        if (s1.find(shingle) != s1.end()) numIntersections++;
        else totalShingles++;
    }

    return numIntersections / totalShingles;
}

double similitudShingles(int shingleNumi, int shingleNumj, int baseShingleNum){
    double pi = shingleNumi/baseShingleNum;
    double pj = shingleNumj/baseShingleNum;

    return (pi * pj)/((pi + pj) - (pi * pj));
}

//////////////////////////////////////// MINHASH ///////////////////////////////////////

vector<uint64_t> getMinhashSignature(const ShingleSet& shingles, uint64_t numHashes) {
    vector<uint64_t> signature(numHashes);
    for (uint64_t i = 0; i < numHashes; ++i) {
        // Inicializamos a "infinito"
        uint64_t minhash = numeric_limits<uint64_t>::max();

        // Calculamos el hash de cada shingle y nos quedamos con el minimo
        for (const string& shingle : shingles) {
            uint64_t newHash = xxh64::hash(shingle.c_str(), shingle.size(), i);
            if (newHash < minhash) minhash = newHash;
        }
        signature[i] = minhash;
    }
    return signature;
}

double minhashJaccardSimilarity(const vector<uint64_t>& s1, const vector<uint64_t>& s2) {
    double matches = 0;
    int numHashes = s1.size();
    for (int i = 0; i < numHashes; ++i) 
        if (s1[i] == s2[i]) matches++;
    return matches / numHashes;
}

////////////////////////////////////////// LSH //////////////////////////////////////////

// aun por retocar
struct LSHParams {
    int bands;      // Número de bandas
    int rows;       // Filas por banda
    double t;       // Umbral teórico (para registro)
    int numHashes;  // Total de hashes (n = bands*rows)
};

struct LSHResults {
    double recall = 0.0;
    double precision = 0.0;
    int truePositives =  0;
    int falsePositives = 0;
    int falseNegatives = 0;
    map<string, double> timings;
    vector<pair<int,int>> candidatePairs;
};

LSHResults applyLSH(
    const vector<vector<uint64_t>>& signatures, // Firmas MinHash de todos los documentos
    const LSHParams& params,
    const vector<ShingleSet>& shingles,         // Shingles originales para calculo de Jaccard real
    double similarity_threshold                 // Umbral real para considerar similares
) {
    LSHResults results;
    auto startHashing = chrono::high_resolution_clock::now();
    
    // 1. Validación de parámetros
    if (params.bands * params.rows != signatures[0].size()) {
        cout << "Error: bands*rows debe ser igual al tamaño de la firma" << endl;
        exit(1);
    }

    //////////////////////////////// CREAR BANDAS & HASHEAR ////////////////////////////////

    vector<unordered_map<string, vector<int>>> bands(params.bands);
    int docID = 0;
    
    // Iteramos sobre todos los documentos (signatures)
    for (const auto& signature : signatures) {
        for (int band = 0; band < params.bands; ++band) {
            // Extraemos la subsignature
            vector<uint64_t> sub_signature(
                signature.begin() + band * params.rows,
                signature.begin() + (band + 1) * params.rows
            );
            
            // Hashear la subfirma
            string hash = "";
            for (auto val : sub_signature) {
                hash += to_string(val) + "|";
            }
            uint64_t band_hash = xxh64::hash(hash.c_str(), hash.size(), band);
            
            // Almacenar en la banda
            bands[band][to_string(band_hash)].push_back(docID);
        }
        ++docID;
    }
    
    auto endHashing = chrono::high_resolution_clock::now();
    results.timings["Hashing"] = chrono::duration<double>(endHashing - startHashing).count();
    
    /////////////////////////////// GENERAR PARES CANDIDATOS ///////////////////////////////

    unordered_set<string> unique_pairs;
    
    for (const auto& band : bands) {
        for (const auto& bucket : band) {
            const auto& docs = bucket.second;
            for (size_t i = 0; i < docs.size(); ++i) {
                for (size_t j = i+1; j < docs.size(); ++j) {
                    int a = min(docs[i], docs[j]);
                    int b = max(docs[i], docs[j]);
                    unique_pairs.insert(to_string(a) + "-" + to_string(b));
                }
            }
        }
    }
    
    /////////////////////////////////// PARES TO INDICES ///////////////////////////////////

    for (const auto& pair_str : unique_pairs) {
        auto dash_pos = pair_str.find('-');
        results.candidatePairs.emplace_back(
            stoi(pair_str.substr(0, dash_pos)),
            stoi(pair_str.substr(dash_pos+1))
        );
    }
    
    auto endCandidates = chrono::high_resolution_clock::now();
    results.timings["CandidatesGeneration"] = chrono::duration<double>(endCandidates - endHashing).count();
    
    //////////////////////////////// VERIFICACION DE PARES ////////////////////////////////

    unordered_set<string> true_similar_pairs;
    for (size_t i = 0; i < shingles.size(); ++i) {
        for (size_t j = i+1; j < shingles.size(); ++j) {
            double sim = shinglesJaccardSimilarity(shingles[i], shingles[j]);
            if (sim >= similarity_threshold) {
                true_similar_pairs.insert(to_string(i) + "-" + to_string(j));
            }
        }
    }
    
    ///////////////////////////////// CALCULO DE METRICAS /////////////////////////////////

    for (const auto& pair : results.candidatePairs) {
        string pair_str = to_string(pair.first) + "-" + to_string(pair.second);
        
        if (true_similar_pairs.count(pair_str)) {
            results.truePositives++;
        }
        else ++results.falsePositives;
    }
    
    results.falseNegatives = true_similar_pairs.size() - results.truePositives;
    
    ///////////////////////////// CALCULO DE PRECISION Y RECALL /////////////////////////////
    if (!results.candidatePairs.empty()) {
        results.precision = static_cast<double>(results.truePositives) / results.candidatePairs.size();
    }
    
    if (!true_similar_pairs.empty()) {
        results.recall = static_cast<double>(results.truePositives) / true_similar_pairs.size();
    }
    
    auto endTotal = chrono::high_resolution_clock::now();
    results.timings["Total"] = chrono::duration<double>(endTotal - startHashing).count();
    
    return results;
}