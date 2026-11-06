// Per a cada metode i cada n de QuantitatDades.txt, executa l'algorisme 10 vegades,
// en calcula la mitjana i desa els resultats a FormatDades/.
//
// Execucio (des de l'arrel del repositori):  make run P=mesura

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int METODES = 3;
const int EXECUCIONS = 10;
const double LIMIT_SEGONS = 60;   // Si una execucio el supera, el metode no prova cap n mes gran
const long long SENSE_DADA = -1;  // Marca una n sense mesurar (a la taula conjunta surt NA)


// ---- ALGORISMES: substituiu el contingut de cada funcio ---------------------
unsigned long long metode1(long long n) {
    return n;  // TODO: algorisme 1
}

unsigned long long metode2(long long n) {
    return n;  // TODO: algorisme 2
}

unsigned long long metode3(long long n) {
    return n;  // TODO: algorisme 3
}
// -----------------------------------------------------------------------------


// Executa l'algorisme del metode indicat
unsigned long long executar(int metode, long long n) {
    if (metode == 1) return metode1(n);
    if (metode == 2) return metode2(n);
    return metode3(n);
}


// Retorna el que triga una execucio, en nanosegons
long long mesurar_temps(int metode, long long n) {
    // volatile: evita que el compilador elimini el calcul o el tregui del cronometre
    volatile long long entrada = n;

    auto inici = chrono::steady_clock::now();
    volatile unsigned long long resultat = executar(metode, entrada);
    auto fi = chrono::steady_clock::now();

    (void)resultat;
    return chrono::duration_cast<chrono::nanoseconds>(fi - inici).count();
}


// Llegeix els valors de n del fitxer, que ha de tenir nomes nombres
vector<long long> llegir_valors_n() {
    vector<long long> valors_n;
    ifstream fitxer("FormatDades/QuantitatDades.txt");

    long long n;
    while (fitxer >> n) {
        valors_n.push_back(n);
    }

    return valors_n;
}


// Mesura les 10 execucions d'una n. Si se supera el limit, retorna el vector buit
vector<long long> mesurar_n(int metode, long long n, bool& limit_superat) {
    vector<long long> temps;

    for (int i = 0; i < EXECUCIONS; i++) {
        long long t = mesurar_temps(metode, n);

        if (t > LIMIT_SEGONS * 1e9) {
            limit_superat = true;
            return {};
        }

        temps.push_back(t);
    }

    return temps;
}


// Calcula la mitjana dels temps, arrodonida a l'enter
long long calcular_mitjana(const vector<long long>& temps) {
    long long suma = 0;
    for (long long t : temps) {
        suma += t;
    }

    return llround((double)suma / temps.size());
}


// Desa els temps d'una n: metodeM/Inputs/dades_nN.txt
void escriure_temps(int metode, long long n, const vector<long long>& temps) {
    ofstream fitxer("FormatDades/metode" + to_string(metode) + "/Inputs/dades_n" + to_string(n) + ".txt");

    fitxer << "n temps_ns" << endl;
    for (long long t : temps) {
        fitxer << n << " " << t << endl;
    }
}


// Desa la taula d'un metode: metodeM/Outputs/dades_metodeM.txt
void escriure_taula_metode(int metode, const vector<long long>& valors_n, const vector<long long>& mitjanes) {
    string nom = "metode" + to_string(metode);
    ofstream fitxer("FormatDades/" + nom + "/Outputs/dades_" + nom + ".txt");

    fitxer << "n temps_ns" << endl;
    for (int i = 0; i < (int)valors_n.size(); i++) {
        if (mitjanes[i] != SENSE_DADA) {
            fitxer << valors_n[i] << " " << mitjanes[i] << endl;
        }
    }
}


// Desa totes les mitjanes juntes per a R: dades_totes.txt
void escriure_taula_conjunta(const vector<long long>& valors_n, const vector<vector<long long>>& mitjanes) {
    ofstream fitxer("FormatDades/dades_totes.txt");

    fitxer << "n metode1 metode2 metode3" << endl;
    for (int i = 0; i < (int)valors_n.size(); i++) {
        fitxer << valors_n[i];

        for (int m = 0; m < METODES; m++) {
            if (mitjanes[m][i] == SENSE_DADA) fitxer << " NA";
            else fitxer << " " << mitjanes[m][i];
        }

        fitxer << endl;
    }
}


int main() {
    vector<long long> valors_n = llegir_valors_n();

    if (valors_n.empty()) {
        cout << "Error: no es pot llegir FormatDades/QuantitatDades.txt" << endl;
        return 1;
    }

    // Una fila per metode i una columna per n
    vector<vector<long long>> mitjanes(METODES, vector<long long>(valors_n.size(), SENSE_DADA));

    for (int metode = 1; metode <= METODES; metode++) {
        string carpeta = "FormatDades/metode" + to_string(metode);
        filesystem::create_directories(carpeta + "/Inputs");
        filesystem::create_directories(carpeta + "/Outputs");

        bool limit_superat = false;

        for (int i = 0; i < (int)valors_n.size(); i++) {
            long long n = valors_n[i];
            vector<long long> temps;

            if (!limit_superat) {
                cout << "metode" << metode << " | n = " << n << " ..." << flush;
                temps = mesurar_n(metode, n, limit_superat);

                if (limit_superat) {
                    cout << " limit de " << LIMIT_SEGONS << " s superat; es descarta aquesta n i les seguents" << endl;
                } else {
                    mitjanes[metode - 1][i] = calcular_mitjana(temps);
                    cout << " " << mitjanes[metode - 1][i] << " ns" << endl;
                }
            }

            // Primer es mesura i despres s'escriu, perque escriure no afecti els temps.
            // Es desa despres de cada n: si s'atura amb Ctrl+C, no es perd el que ja s'ha mesurat.
            escriure_temps(metode, n, temps);
            escriure_taula_metode(metode, valors_n, mitjanes[metode - 1]);
            escriure_taula_conjunta(valors_n, mitjanes);
        }
    }

    cout << "Fet. Taula conjunta: FormatDades/dades_totes.txt" << endl;

    return 0;
}
