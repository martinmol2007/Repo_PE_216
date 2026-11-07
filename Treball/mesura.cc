// Mesura tots els metodes amb totes les n de QuantitatDades.txt, 10 vegades cadascuna.
// Totes les execucions (metode, n i repeticio) es fan en un unic ordre aleatori.
// Calcula la mitjana de cada n i metode i desa els resultats a FormatDades/.
//
// Execucio (des de l'arrel del repositori):  make run P=mesura

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int METODES = 3;
const int EXECUCIONS = 10;
const unsigned LLAVOR = 216;      // Llavor de l'ordre aleatori (numero del grup)
const double LIMIT_SEGONS = 60;   // Si una execucio el supera, el metode descarta aquesta n i les mes grans
const long long SENSE_DADA = -1;  // Marca una n sense mesurar (a la taula conjunta surt NA)

// n mes gran que prova cada metode. Cal perque, en ordre aleatori, un algorisme
// exponencial podria tocar una n enorme abans que el limit de temps l'aturi.
const long long N_MAXIMA[METODES] = {
    45,        // metode1: recursio basica
    10000000,  // metode2: programacio dinamica
    10000000,  // metode3: duplicacio rapida
};


// ---- ALGORISMES: substituiu el contingut de cada funcio ---------------------
// noipa no s'ha de treure: sense ell, g++ -O2 simplifica la recursio basica
// (amb n = 44 passa de 2 s a 2 ms) i no es mesuraria l'algorisme real.
__attribute__((noipa)) unsigned long long metode1(long long n) {
    return n;  // TODO: recursio basica
}

__attribute__((noipa)) unsigned long long metode2(long long n) {
    return n;  // TODO: programacio dinamica
}

__attribute__((noipa)) unsigned long long metode3(long long n) {
    return n;  // TODO: duplicacio rapida
}
// -----------------------------------------------------------------------------


// Una execucio pendent. index_n es la posicio de la n dins de valors_n
struct Execucio {
    int metode;
    int index_n;
    int repeticio;
};


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


// Crea la llista de totes les execucions i la barreja amb la llavor fixa
vector<Execucio> crear_ordre_aleatori(const vector<long long>& valors_n) {
    vector<Execucio> ordre;

    for (int metode = 1; metode <= METODES; metode++) {
        for (int i = 0; i < (int)valors_n.size(); i++) {
            if (valors_n[i] > N_MAXIMA[metode - 1]) continue;

            for (int repeticio = 1; repeticio <= EXECUCIONS; repeticio++) {
                ordre.push_back({metode, i, repeticio});
            }
        }
    }

    // Barreja de Fisher-Yates: dona el mateix ordre a tots els ordinadors
    mt19937 generador(LLAVOR);
    for (int i = (int)ordre.size() - 1; i > 0; i--) {
        swap(ordre[i], ordre[generador() % (i + 1)]);
    }

    return ordre;
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
    vector<vector<vector<long long>>> temps(METODES, vector<vector<long long>>(valors_n.size()));

    // La primera n que cada metode ja no ha de provar
    vector<long long> n_descartada(METODES);

    // Es deixen tots els fitxers buits perque no quedin resultats d'una mesura anterior
    for (int metode = 1; metode <= METODES; metode++) {
        string carpeta = "FormatDades/metode" + to_string(metode);
        filesystem::create_directories(carpeta + "/Inputs");
        filesystem::create_directories(carpeta + "/Outputs");

        n_descartada[metode - 1] = N_MAXIMA[metode - 1] + 1;

        for (long long n : valors_n) {
            escriure_temps(metode, n, {});
        }
        escriure_taula_metode(metode, valors_n, mitjanes[metode - 1]);
    }
    escriure_taula_conjunta(valors_n, mitjanes);

    vector<Execucio> ordre = crear_ordre_aleatori(valors_n);
    ofstream registre("FormatDades/ordre_execucio.txt");
    registre << "ordre metode n repeticio temps_ns" << endl;

    for (int k = 0; k < (int)ordre.size(); k++) {
        int metode = ordre[k].metode;
        int m = metode - 1;
        int i = ordre[k].index_n;
        long long n = valors_n[i];

        if (n >= n_descartada[m]) continue;

        cout << k + 1 << "/" << ordre.size() << " metode" << metode << " | n = " << n
             << " | repeticio " << ordre[k].repeticio << " ..." << flush;

        long long t = mesurar_temps(metode, n);

        // Primer es mesura i despres s'escriu, perque escriure no afecti els temps.
        // Es desa despres de cada execucio: si s'atura amb Ctrl+C, no es perd el que ja s'ha mesurat.
        cout << " " << t << " ns" << endl;
        registre << k + 1 << " " << metode << " " << n << " " << ordre[k].repeticio << " " << t << endl;

        if (t > LIMIT_SEGONS * 1e9) {
            cout << "Limit de " << LIMIT_SEGONS << " s superat: metode" << metode
                 << " descarta n = " << n << " i les mes grans" << endl;
            n_descartada[m] = n;

            for (int j = 0; j < (int)valors_n.size(); j++) {
                if (valors_n[j] >= n && !temps[m][j].empty()) {
                    temps[m][j].clear();
                    mitjanes[m][j] = SENSE_DADA;
                    escriure_temps(metode, valors_n[j], {});
                }
            }
        } else {
            temps[m][i].push_back(t);
            escriure_temps(metode, n, temps[m][i]);

            // Una n nomes te mitjana quan s'han fet totes les repeticions
            if ((int)temps[m][i].size() < EXECUCIONS) continue;

            mitjanes[m][i] = calcular_mitjana(temps[m][i]);
        }

        escriure_taula_metode(metode, valors_n, mitjanes[m]);
        escriure_taula_conjunta(valors_n, mitjanes);
    }

    cout << "Fet. Taula conjunta: FormatDades/dades_totes.txt" << endl;

    return 0;
}
