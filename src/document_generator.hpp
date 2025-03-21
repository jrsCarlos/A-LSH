#ifndef DOCUMENT_GENERATOR_HPP
#define DOCUMENT_GENERATOR_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <set>

using namespace std;

vector<string> splitText(const string& text);

string generatePermutedText(const string& text);

#endif // DOCUMENT_GENERATOR_HPP