#include <iostream>
#include <fstream>
#include "json.hpp"
#include <set>

using namespace std;
using namespace nlohmann;

int main() {

}



/*
    // Leer el archivo JSON
    ifstream inputFile("./deps/stopwords-iso.json");
    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    json datos;
    inputFile >> datos;  // Cargar el contenido del archivo en el objeto JSON

    // Claves permitidas
    set<string> claves_permitidas = {"es", "ca", "en"};

    // Iterar sobre las claves y eliminar las no permitidas
    for (auto it = datos.begin(); it != datos.end();) {
        if (claves_permitidas.find(it.key()) == claves_permitidas.end()) {
            it = datos.erase(it);  // Eliminar el elemento y continuar con el siguiente
        }
        else ++it;
    }

    // Guardar el JSON modificado de nuevo en el archivo
    ofstream outputFile("stopwords.json");
    outputFile << datos.dump();
    return 0; 
*/