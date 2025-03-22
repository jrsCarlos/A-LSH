#include "hashing_tools.hpp"
#include <cassert>

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
struct pair_hash {
    size_t operator() (const pair<int, int>& p) const {
        auto h1 = hash<int>{}(p.first);
        auto h2 = hash<int>{}(p.second);
        // Combina los dos hashes
        return h1 ^ h2; 
    }
};

// aun por retocar
vector<pair<int,int>> LSH(const vector<vector<uint64_t>>& signatures, int numBands, int numRows) {
    //////////////////////////////// CREAR BANDAS & HASHEAR ////////////////////////////////
    
    // vector<unordered_map<string, vector<int>>>: Almacena todas las bandas del LSH
    // unordered_map<unint64_t, vector<int>>: Almacena todos los documentos que comparten el mismo hash en una banda
    // vector<int>: Almacena los IDs que comparten el mismo hash en una banda
    
    vector<unordered_map<uint64_t, vector<int>>> bands(numBands);
    int docID = 0;
    
    // Iteramos sobre todos los documentos (signatures)
    for (const auto& signature : signatures) {
        for (int band = 0; band < numBands; ++band) {
            // Claculamos la posicion del primer elemento de la subfirma
            auto subSignature = signature.data() + band * numRows;
            
            // Hasheamos todos los elementos de la subfirma
            const char *ptr = reinterpret_cast<const char*>(subSignature);
            uint64_t length = numRows * sizeof(uint64_t);
            uint64_t bandHash = xxh64::hash(ptr, length, band);
            
            // Almacenar en la banda
            bands[band][bandHash].push_back(docID);
        }
        ++docID;
    }
        
    /////////////////////////////// GENERAR PARES CANDIDATOS ///////////////////////////////

    unordered_set<pair<int,int>, pair_hash> uniquePairs;
    
    for (const auto& band : bands) {
        for (const auto& bucket : band) {
            const auto& docs = bucket.second;
            int numDocs = docs.size();

            for (int i = 0; i < numDocs; ++i) {
                for (int j = i+1; j < numDocs; ++j) {
                    int a = min(docs[i], docs[j]);
                    int b = max(docs[i], docs[j]);
                    uniquePairs.insert(make_pair(a,b));
                }
            }
        }
    }
    
    /////////////////////////////////// PARES TO INDICES ///////////////////////////////////

    vector<pair<int,int>> candidatePairs;
    for (const auto& pair : uniquePairs) candidatePairs.push_back(pair);
    return candidatePairs;
}