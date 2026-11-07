// Tria a l'atzar els 30 valors de n i els desa a FormatDades/QuantitatDades.txt.
// El rang es divideix en 6 trams i de cada tram s'escullen 5 valors diferents.
// Amb la mateixa llavor surten sempre els mateixos valors.
//
// Execucio (des de l'arrel del repositori):  make run P=tria_n

#include <fstream>
#include <iostream>
#include <random>
#include <set>

using namespace std;

const unsigned LLAVOR = 216;  // Numero del grup
const int VALORS_PER_TRAM = 5;

// Els dos primers trams son el rang on la recursio basica es executable (n <= 45)
const long long TRAMS[][2] = {
    {20, 32},
    {33, 45},
    {46, 1000},
    {1001, 20000},
    {20001, 500000},
    {500001, 10000000},
};


int main() {
    mt19937 generador(LLAVOR);
    ofstream fitxer("FormatDades/QuantitatDades.txt");

    if (!fitxer) {
        cout << "Error: no es pot escriure FormatDades/QuantitatDades.txt" << endl;
        return 1;
    }

    for (const auto& tram : TRAMS) {
        long long minim = tram[0];
        long long maxim = tram[1];

        // set: no repeteix valors i els deixa ordenats de petit a gran
        set<long long> valors;
        while ((int)valors.size() < VALORS_PER_TRAM) {
            valors.insert(minim + generador() % (maxim - minim + 1));
        }

        for (long long n : valors) {
            fitxer << n << endl;
        }
    }

    cout << "Fet. Valors de n a FormatDades/QuantitatDades.txt" << endl;

    return 0;
}
