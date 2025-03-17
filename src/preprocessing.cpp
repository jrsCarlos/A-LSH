#include "preprocessing.hh"

using json = nlohmann::json;
using namespace std;

// Función para cargar las stopwords desde el JSON
unordered_set<string> loadStopwords(const string& idioma) {
    ifstream inputFile("deps/stopwords.json");  // Abre el inputFile
    if (!inputFile) {
        cerr << "No se pudo abrir el inputFile JSON.\n";
    }

    json json_data;
    inputFile >> json_data;

    unordered_set<string> stopwords;
    if (json_data.contains(idioma)) {
        vector<string> stopwords_vector = json_data[idioma].get<vector<string>>();
        stopwords.insert(stopwords_vector.begin(), stopwords_vector.end());
    }
    return stopwords;
}

void removeStopwords(string& text, const string& idioma) {
    unordered_set<string> stopwords = loadStopwords(idioma);
    
    stringstream ss(text);
    string palabra;
    string cleanText;

    while (ss >> palabra) {
        if (stopwords.find(palabra) == stopwords.end()) {
            if (!cleanText.empty()) cleanText += " ";
            cleanText += palabra;
        }
    }

    text = cleanText;
}

void cleanText(string& text, unordered_set<char>& punctuationMarks) {
    size_t write_idx = 0;
    bool space = false;

    for (size_t i = 0; i < text.size(); ++i) {
        // Ignoramos los salto de linea y tabulaciones
        if (text[i] == '\t' || text[i] == '\n') {
            if (!space) {
                text[write_idx++] = ' ';  // Sustituye por un solo espacio
                space = true;
            }
            continue;
        }

        // Eliminar signos de puntuación
        if (!punctuationMarks.count(text[i])) {
            // Comprimimos espacios consecutivos
            if (text[i] == ' ') {
                // Si no hay un espacio previo, agregamos uno
                if (!space) {
                    text[write_idx++] = text[i];
                    space = true;
                }
            }
            else {
                // Agregamos el caracter
                text[write_idx++] = text[i];
                space = false;
            }
        }
    }
    
    text.erase(write_idx);
}

void toLowercase(string& text) {
    wstring noLowercaseText = wstring(text.begin(), text.end());
    transform(noLowercaseText.begin(), noLowercaseText.end(), noLowercaseText.begin(), towlower);
    text = string(noLowercaseText.begin(), noLowercaseText.end());
}