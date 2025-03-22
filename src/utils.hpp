#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

void saveDocument(const string& docText, const string& docPath);
vector<fs::path> getDocs(const string& docsDir);
string loadDocument(const string& docPath);


#endif