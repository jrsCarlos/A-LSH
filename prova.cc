#include <iostream>
#include <fstream>
#include <json.hpp>

using namespace std;
using namespace nlohmann;

int main() {
    ifstream archivo("stopwords-iso.json");
    json datos;
    archivo >> datos;

    cout << "Spanish: " << datos["es"] << endl;

    return 0;
}
