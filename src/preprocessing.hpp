#ifndef PREPROCESSING_HH
#define PREPROCESSING_HH

#include "json.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_set>

using json = nlohmann::json;
using namespace std;

// Carga las stopwords de un idioma
// En el json de las stopwords, tenemos ES, EN y CA
unordered_set<string> loadStopwords(const string& idioma);

string getLanguage(const string& text);

// Limpia el texto de signos de puntuacion y caracteres especiales
void cleanText(string& text);

// Convierte el texto a minusculas
// Como no hay manera de tratar todos los idiomas existentes y sus letras minusculas y mayusculas, lo hacemos 
// a mano con los idiomas que trateremos, ingles, catalan, castellano.
void toLowercase(string& text);

void removeAccents(string& text);

// Elimina las stopwords del texto
void removeStopwords(string& text, const string& idioma);


#endif