#ifndef HASHING_TOOLS_HPP
#define HASHING_TOOLS_HPP

#include <string>
#include <vector>
#include <limits>
#include <unordered_set>
#include "xxh64.hpp"
using namespace std;

void generateShingles(unordered_set<string>& shingles, const string& text, int k);

double shinglesJaccardSimilarity(const unordered_set<string>& s1, const unordered_set<string>& s2);

vector<uint64_t> getMinhashSignature(const unordered_set<string>& shingles, uint64_t numHashes);

#endif