#include "preprocessing.hh"

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
