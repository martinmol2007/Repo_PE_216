// Experiment complet: per a cada metode i cada n de FormatDades/QuantitatDades.txt,
// executa l'algorisme 10 vegades, en calcula la mitjana i desa els resultats.
//
// Execucio (des de l'arrel del repositori):  make run P=mesura
//
// Fitxers que genera dins de FormatDades/ (M = metode, N = valor de n):
//     metodeM/Inputs/dades_nN.txt         els 10 temps de cada n
//     metodeM/Outputs/dades_metodeM.txt   la mitjana de cada n del metode
//     dades_totes.txt                     totes les mitjanes juntes, per llegir amb R
//
// Cada execucio sobreescriu els resultats anteriors.

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

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

using Algorisme = unsigned long long (*)(long long);

const int METODES = 3;
const Algorisme ALGORISMES[METODES] = {metode1, metode2, metode3};
const int EXECUCIONS = 10;

// Si una execucio supera aquest temps, el metode no prova cap n mes gran. Aixi un
// algorisme exponencial no es queda bloquejat amb les n grans de la llista.
const double LIMIT_SEGONS = 60;

const long long SENSE_DADA = -1;
const fs::path DADES = "FormatDades";

// Les variables volatile eviten que el compilador (-O2) elimini el calcul
// o el mogui fora de la zona cronometrada.
long long mesura_ns(Algorisme algorisme, long long n) {
    volatile long long entrada = n;
    volatile unsigned long long sortida = 0;

    auto inici = std::chrono::steady_clock::now();
    sortida = algorisme(entrada);
    auto fi = std::chrono::steady_clock::now();

    (void)sortida;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(fi - inici).count();
}

// Llegeix les linies formades nomes per digits i ignora els comentaris del fitxer.
// Les n s'ordenen de petita a gran perque LIMIT_SEGONS descarti les mes costoses.
std::vector<long long> llegeix_valors_n() {
    std::ifstream fitxer(DADES / "QuantitatDades.txt");
    std::vector<long long> valors;
    std::string linia;
    while (std::getline(fitxer, linia)) {
        if (!linia.empty() && linia.back() == '\r') linia.pop_back();
        bool nomes_digits = !linia.empty() && std::all_of(linia.begin(), linia.end(), [](unsigned char c) {
            return c >= '0' && c <= '9';
        });
        if (nomes_digits) valors.push_back(std::stoll(linia));
    }
    std::sort(valors.begin(), valors.end());
    valors.erase(std::unique(valors.begin(), valors.end()), valors.end());
    return valors;
}

void comprova_escriptura(const std::ofstream& fitxer, const fs::path& cami) {
    if (!fitxer) {
        std::cerr << "Error: no s'ha pogut escriure " << cami.string() << "\n";
        std::exit(1);
    }
}

void escriu_temps(const fs::path& cami, long long n, const std::vector<long long>& temps) {
    fs::create_directories(cami.parent_path());
    std::ofstream fitxer(cami);
    fitxer << "n temps_ns\n";
    for (long long t : temps) fitxer << n << " " << t << "\n";
    fitxer.close();
    comprova_escriptura(fitxer, cami);
}

void escriu_taula_metode(const fs::path& cami, const std::vector<long long>& ns,
                         const std::vector<long long>& mitjanes) {
    fs::create_directories(cami.parent_path());
    std::ofstream fitxer(cami);
    fitxer << "n temps_ns\n";
    for (size_t i = 0; i < ns.size(); ++i) {
        if (mitjanes[i] != SENSE_DADA) fitxer << ns[i] << " " << mitjanes[i] << "\n";
    }
    fitxer.close();
    comprova_escriptura(fitxer, cami);
}

// Una fila per n i una columna per metode, l'estructura de dades aparellades de la
// guia 01_Dades. Els valors sense mesurar surten com a NA, el valor absent de R.
void escriu_taula_conjunta(const fs::path& cami, const std::vector<long long>& ns,
                           const std::array<std::vector<long long>, METODES>& mitjanes) {
    std::ofstream fitxer(cami);
    fitxer << "n";
    for (int m = 1; m <= METODES; ++m) fitxer << " metode" << m;
    fitxer << "\n";
    for (size_t i = 0; i < ns.size(); ++i) {
        fitxer << ns[i];
        for (int m = 0; m < METODES; ++m) {
            fitxer << " ";
            if (mitjanes[m][i] == SENSE_DADA) fitxer << "NA";
            else fitxer << mitjanes[m][i];
        }
        fitxer << "\n";
    }
    fitxer.close();
    comprova_escriptura(fitxer, cami);
}

int main() {
    if (!fs::is_directory(DADES)) {
        std::cerr << "Error: no es troba FormatDades. Executa amb: make run P=mesura\n";
        return 1;
    }

    const std::vector<long long> ns = llegeix_valors_n();
    if (ns.empty()) {
        std::cerr << "Error: no hi ha cap valor de n a QuantitatDades.txt\n";
        return 1;
    }

    std::array<std::vector<long long>, METODES> mitjanes;
    for (auto& columna : mitjanes) columna.assign(ns.size(), SENSE_DADA);

    const long long limit_ns = static_cast<long long>(LIMIT_SEGONS * 1e9);

    for (int m = 0; m < METODES; ++m) {
        const std::string nom = "metode" + std::to_string(m + 1);
        const fs::path base = DADES / nom;
        bool limit_superat = false;

        for (size_t i = 0; i < ns.size(); ++i) {
            const long long n = ns[i];
            std::vector<long long> temps;

            if (!limit_superat) {
                std::cout << nom << " | n = " << n << " ..." << std::flush;
                for (int e = 0; e < EXECUCIONS; ++e) {
                    long long t = mesura_ns(ALGORISMES[m], n);
                    temps.push_back(t);
                    if (t > limit_ns) {
                        limit_superat = true;
                        temps.clear();
                        std::cout << " limit de " << LIMIT_SEGONS << " s superat; es descarta"
                                  << " aquesta n i les seguents\n";
                        break;
                    }
                }
            }

            // Primer es mesura i despres s'escriu, perque l'escriptura no afecti els temps.
            escriu_temps(base / "Inputs" / ("dades_n" + std::to_string(n) + ".txt"), n, temps);

            if (!temps.empty()) {
                long double suma = 0;
                for (long long t : temps) suma += t;
                mitjanes[m][i] = std::llround(suma / temps.size());
                std::cout << " " << mitjanes[m][i] << " ns\n";
            }
        }

        escriu_taula_metode(base / "Outputs" / ("dades_" + nom + ".txt"), ns, mitjanes[m]);
        // Es reescriu despres de cada metode: si s'interromp, ja hi ha els acabats.
        escriu_taula_conjunta(DADES / "dades_totes.txt", ns, mitjanes);
    }

    std::cout << "Fet. Taula conjunta: " << (DADES / "dades_totes.txt").string() << "\n";
    return 0;
}
