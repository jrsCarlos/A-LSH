#ifndef HASHING_TOOLS_HPP
#define HASHING_TOOLS_HPP

#include <string>
#include <vector>
#include <limits>
#include "xxh64.hpp"
#include <unordered_set>

using namespace std;
using ShingleSet = unordered_set<string>;

/////////////////////////////////////// SHINGLES ///////////////////////////////////////

void generateShingles(ShingleSet& shingles, const string& text, int k);

double shinglesJaccardSimilarity(const ShingleSet& s1, const ShingleSet& s2);

//////////////////////////////////////// MINHASH ///////////////////////////////////////

vector<uint64_t> getMinhashSignature(const ShingleSet& shingles, uint64_t numHashes);

double minhashJaccardSimilarity(const vector<uint64_t>& s1, const vector<uint64_t>& s2);

////////////////////////////////////////// LSH //////////////////////////////////////////

#endif