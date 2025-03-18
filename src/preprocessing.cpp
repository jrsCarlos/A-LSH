#include "preprocessing.hpp"

unordered_set<string> loadStopwords(const string& language) {
    string filePath = "deps/stopwords.json";
    ifstream jsonFile(filePath);
    if (!jsonFile) cerr << "No se pudo leer " << filePath << endl;

    json jsonStopwords;
    jsonFile >> jsonStopwords;

    unordered_set<string> stopwords;
    vector<string> vectorStopwords = jsonStopwords[language].get<vector<string>>();
    stopwords.insert(vectorStopwords.begin(), vectorStopwords.end());

    return stopwords;
}

void removeStopwords(string& text, const string& language) {
    unordered_set<string> stopwords = loadStopwords(language);
    
    stringstream ss(text);
    string word;
    string noStopwordsText;

    while (ss >> word) {
        // Si no es una stopword, la agregamos al texto
        if (stopwords.find(word) == stopwords.end()) {
            // Agregamos espacios para que no se junten las palabras
            if (!noStopwordsText.empty()) noStopwordsText += " ";
            noStopwordsText += word;
        }
    }

    text = noStopwordsText;
}

void cleanText(string& text) {
    string symbols = ".,;:¿?¡!\"'()_-[]{}—…‐‑‒–—―/\\«»‹›‘’“”";
    unordered_set<char> punctuationMarks(symbols.begin(), symbols.end());

    // Indice que indica donde se va a escribir en el texto
    size_t writeIndex = 0;
    bool space = false;

    for (size_t i = 0; i < text.size(); ++i) {
        // Sustituimos tabulaciones (\t) y saltos de linea (\n) por espacios
        if (text[i] == '\t' || text[i] == '\n') {
            if (!space) {
                text[writeIndex++] = ' ';
                space = true;
            }
            continue;
        }

        // Eliminar signos de puntuacion
        if (!punctuationMarks.count(text[i])) {
            // Comprimimos espacios consecutivos
            if (text[i] == ' ') {
                // Si no hay un espacio previo, agregamos uno
                if (!space) {
                    text[writeIndex++] = text[i];
                    space = true;
                }
            }
            else {
                // Agregamos el caracter
                text[writeIndex++] = text[i];
                space = false;
            }
        }
    }
    
    // Eliminamos los caracteres sobrantes
    text.erase(writeIndex);
}

void toLowercase(string& text) {
    // Conversion a wide string
    wstring lowercaseText = wstring(text.begin(), text.end());

    // Reemplazamos las mayusculas por minusculas
    transform(lowercaseText.begin(), lowercaseText.end(), lowercaseText.begin(), towlower);

    // Conversion a string
    text = string(lowercaseText.begin(), lowercaseText.end());
}