#ifndef PREPROCESSING_HH
#define PREPROCESSING_HH

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

string removeStopwords(const string& text);


#endif