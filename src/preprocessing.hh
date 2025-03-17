#ifndef PREPROCESSING_HH
#define PREPROCESSING_HH

#include "json.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_set>

using namespace std;
using namespace nlohmann;

void cleanText(string& text, unordered_set<char>& punctuationMarks);

unordered_set<string> loadStopwords(const string& idioma);
void removeStopwords(string& text, const string& idioma);

void toLowercase(string& text);

#endif