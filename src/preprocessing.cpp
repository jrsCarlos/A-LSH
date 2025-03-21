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

string getLanguage(const string& text) {
    // Contamos las ocurrencias de las stopwords de cada idioma
    vector<string> languages = {"en", "ca", "es"};
    unordered_set<string> stopwords;

    int languageIdx = 0;
    int maxCount = -1;

    int numLanguages = languages.size();
    for (int i = 0; i < numLanguages; i++) {
        stopwords = loadStopwords(languages[i]);
        int count = 0;

        stringstream ss(text);
        string word;

        while (ss >> word) {
            if (stopwords.count(word)) ++count;
        }

        if (count > maxCount) {
            maxCount = count;
            languageIdx = i;
        }
    }
    return languages[languageIdx];
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
    map<char,char> specialLetters = {
        {'À', 'à'}, {'Á', 'á'}, {'Â', 'â'}, {'Ä', 'ä'},
        {'È', 'è'}, {'É', 'é'}, {'Ê', 'ê'}, {'Ë', 'ë'},
        {'Ì', 'ì'}, {'Í', 'í'}, {'Î', 'î'}, {'Ï', 'ï'},
        {'Ò', 'ò'}, {'Ó', 'ó'}, {'Ô', 'ô'}, {'Ö', 'ö'},
        {'Ù', 'ù'}, {'Ú', 'ú'}, {'Û', 'û'}, {'Ü', 'ü'},
        {'Ç', 'ç'}, {'Ñ', 'ñ'}
    };

    for (char& letter : text) {
        if (specialLetters.count(letter)) letter = specialLetters[letter];
        letter = tolower(letter);
    }
}

void removeAccents(string& text) {
    // Map of accented characters to their non-accented equivalents
    map<char,char> accentLetters = {
        {'à', 'a'}, {'á', 'a'}, {'â', 'a'}, {'ä', 'a'},
        {'è', 'e'}, {'é', 'e'}, {'ê', 'e'}, {'ë', 'e'},
        {'ì', 'i'}, {'í', 'i'}, {'î', 'i'}, {'ï', 'i'},
        {'ò', 'o'}, {'ó', 'o'}, {'ô', 'o'}, {'ö', 'o'},
        {'ù', 'u'}, {'ú', 'u'}, {'û', 'u'}, {'ü', 'u'},
        {'ç', 'c'}, {'ñ', 'n'}
    };

    for (char& letter : text) {
        if (accentLetters.count(letter)) letter = accentLetters[letter];
    }
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
