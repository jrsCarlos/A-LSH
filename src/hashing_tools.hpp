#ifndef HASHING_TOOLS_HPP
#define HASHING_TOOLS_HPP

#include <map>
#include <chrono>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include "xxh64.hpp"
#include <unordered_set>
#include <unordered_map>

using namespace std;
using ShingleSet = unordered_set<string>;

/////////////////////////////////////// SHINGLES ///////////////////////////////////////

void generateShingles(ShingleSet& shingles, const string& text, int k);

double shinglesJaccardSimilarity(const ShingleSet& s1, const ShingleSet& s2);

double similitudShingles(int shingleNumi, int shingleNumj, int baseShingleNum);

ShingleSet obtenerSubconjuntoAleatorio(const ShingleSet& conjunto, size_t tamanoSubconjunto);

//////////////////////////////////////// MINHASH ///////////////////////////////////////

vector<uint64_t> getMinhashSignature(const ShingleSet& shingles, uint64_t numHashes);

double minhashJaccardSimilarity(const vector<uint64_t>& s1, const vector<uint64_t>& s2);

////////////////////////////////////////// LSH //////////////////////////////////////////

vector<pair<int,int>> LSH(const vector<vector<uint64_t>>& signatures, int numBands, int numRows);

#endif