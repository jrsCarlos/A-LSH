#ifndef HASHING_TOOLS_HPP
#define HASHING_TOOLS_HPP

#include <map>
#include <chrono>
#include <string>
#include <vector>
#include <limits>
#include <iostream>
#include "xxh64.hpp"
#include <unordered_set>
#include <unordered_map>

using namespace std;
using ShingleSet = unordered_set<string>;

/////////////////////////////////////// SHINGLES ///////////////////////////////////////

void generateShingles(ShingleSet& shingles, const string& text, int k);

double shinglesJaccardSimilarity(const ShingleSet& s1, const ShingleSet& s2);

//////////////////////////////////////// MINHASH ///////////////////////////////////////

vector<uint64_t> getMinhashSignature(const ShingleSet& shingles, uint64_t numHashes);

double minhashJaccardSimilarity(const vector<uint64_t>& s1, const vector<uint64_t>& s2);

////////////////////////////////////////// LSH //////////////////////////////////////////

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

LSHResults LSH(
    const vector<vector<uint64_t>>& signatures, // Firmas MinHash de todos los documentos
    const LSHParams& params,
    const vector<ShingleSet>& shingles,         // Shingles originales para calculo de Jaccard real
    double simThreshold                 // Umbral real para considerar similares
);

#endif