#include "hashing_tools.hpp"

void generateShingles(unordered_set<string>& shingles, const string& text, int k) {
    size_t length = text.size();
    for (size_t i = 0; i <= length - k; ++i) {
        shingles.insert(text.substr(i, k));
    }
}

double shinglesJaccardSimilarity(const unordered_set<string>& s1, const unordered_set<string>& s2) {
    int intersection_size = 0;
    int union_size = s1.size();

    for (const auto& shingle : s2) {
        if (s1.find(shingle) != s1.end()) {
            intersection_size++;
        }
        else union_size++;
    }

    return (double)intersection_size / union_size;
}

vector<uint64_t> getMinhashSignature(const unordered_set<string>& shingles, uint64_t numHashes) {
    vector<uint64_t> signature(numHashes);
    for (uint64_t i = 0; i < numHashes; ++i) {
        // Inicializamos a "infinito"
        uint64_t minhash = numeric_limits<uint64_t>::max();

        // Calculamos el hash de cada shingle y nos quedamos con el minimo
        for (const string& shingle : shingles) {
            uint64_t hash_val = xxh64::hash(shingle.c_str(), shingle.size(), i);
            if (hash_val < minhash) minhash = hash_val;
        }
        signature[i] = minhash;
    }
    return signature;
}