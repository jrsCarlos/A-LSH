#include "utils.hpp"
#include "hashing_tools.hpp"

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std;

int main() {

    //Get time stamp begining
    auto Tstart = high_resolution_clock::now();





    //Final Timestamp
    auto Tfinal = high_resolution_clock::now();

    
    
    ///////////////////////////////// CALCULO DE TIEMPOS /////////////////////////////////

    //Duration to finish
    auto Dtotal = duration_cast<microseconds>(Tfinal - Tstart);
    cout << "Total time: " << Dtotal.count() << " ms" << endl;


    /* OLD TIME CALC
        //Duration to read
    auto Dread = duration_cast<microseconds>(Tread - Tstart);

    //Duration to generate shingles
    auto Dshingles = duration_cast<microseconds>(Tshingles - Tclean);

    //Duration to minHash
    auto DminHash = duration_cast<microseconds>(TminHash - Tshingles);

    //Duration to finish
    auto Dtotal = duration_cast<microseconds>(Tfinal - Tstart);

    //print times
    cout << endl << "-------------Data-------------" << endl;
    cout << "Text size before cleanup: " << textSizeBC << " bytes/characters" << endl;
    cout << "Text size after cleanup: " << textSizeAC << " bytes/characters" << endl;
    cout << "We removed: " << textSizeBC - textSizeAC << " bytes/characters" << endl;
    cout << "Num of Shingles: " << shinglesNum << " shingles" << endl;
    cout << "Size of Shingles: " << shinglesSize << " bytes" << endl;
    cout << "Num of MinHash: " << minHashNum << " shingles" << endl;
    cout << "Size of MinHash: " << minHashSize << " bytes" << endl;

    cout << endl << "-------------Time-------------" << endl;
    cout << "Time to read files: " << Dread.count() << " ms" << endl;
    cout << "Time to clean: " << Dclean.count() << " ms" << endl;
    cout << "Time to generate shingles: " << Dshingles.count() << " ms" << endl;
    cout << "Time to generate minHash: " << DminHash.count() << " ms" << endl;
    cout << "Total time: " << Dtotal.count() << " ms" << endl;
    */
}
