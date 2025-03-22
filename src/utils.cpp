#include "utils.hpp"

void saveDocument(const string& docText, const string& docPath) {
    ofstream docContent(docPath);
    docContent << docText;
    docContent.close();
}

vector<fs::path> getDocs(const string& docsDir) {
    vector<fs::path> docsNames;
    for (const auto& doc : fs::directory_iterator(docsDir)) {
        if (doc.is_regular_file()) {
            docsNames.push_back(doc.path().filename());
        }
    }
    return docsNames;
}

string loadDocument(const string& docPath) {
    ifstream docContent(docPath);
    stringstream buffer;
    buffer << docContent.rdbuf();
    docContent.close();
    return buffer.str();
}