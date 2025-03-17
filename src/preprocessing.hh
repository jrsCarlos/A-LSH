#ifndef PREPROCESSING_HH
#define PREPROCESSING_HH

#include "json.hpp"
#include <unordered_set>

using namespace std;
using namespace nlohmann;

void cleanText(string& text, unordered_set<char>& punctuationMarks);

#endif