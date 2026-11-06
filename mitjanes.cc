// Calcula la mitjana de cada FormatDades/metodeM/Inputs/dades_nN.txt i reescriu
// la taula FormatDades/metodeM/Outputs/dades_metodeM.txt, ordenada per n i
// llegible per R amb read.table(fitxer, header = TRUE):
//     n temps_ns
//     10 405
//     100 3120
//
// Els fitxers que nomes tenen la capcalera encara no s'han mesurat i no surten a la taula.
//
// Execucio (des de l'arrel del repositori):  make run P=mitjanes
// Es demana el metode (1, 2 o 3).

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>

namespace fs = std::filesystem;

const int EXECUCIONS = 10;

int main() {
    int metode;
    std::cout << "Metode (1, 2 o 3): ";
    if (!(std::cin >> metode) || metode < 1 || metode > 3) {
        std::cerr << "Error: el metode ha de ser 1, 2 o 3.\n";
        return 1;
    }

    std::string nom_metode = "metode" + std::to_string(metode);
    fs::path base = fs::path("FormatDades") / nom_metode;
    fs::path inputs = base / "Inputs";
    if (!fs::is_directory(inputs)) {
        std::cerr << "Error: no existeix " << inputs.string() << ". Executa primer mesura.\n";
        return 1;
    }

    // std::map manté les files ordenades per n (10 abans que 100).
    std::map<long long, long long> mitjanes;
    int sense_mesurar = 0;
    const std::regex patro(R"(dades_n(-?\d+)\.txt)");

    for (const fs::directory_entry& entrada : fs::directory_iterator(inputs)) {
        std::string nom = entrada.path().filename().string();
        std::smatch coincidencia;
        if (!std::regex_match(nom, coincidencia, patro)) continue;

        std::ifstream fitxer(entrada.path());
        std::string capcalera;
        std::getline(fitxer, capcalera);

        const long long n = std::stoll(coincidencia[1].str());
        long double suma = 0;
        int quantitat = 0;
        bool valid = true;
        long long n_fila, temps;
        while (fitxer >> n_fila) {
            // Cada fila ha de tenir un temps i la mateixa n que el nom del fitxer.
            if (!(fitxer >> temps) || n_fila != n) {
                valid = false;
                break;
            }
            suma += temps;
            ++quantitat;
        }
        // Si la lectura s'atura abans del final, hi ha text on hauria d'haver un nombre.
        if (!fitxer.eof()) valid = false;

        if (!valid) {
            std::cerr << "Avis: s'omet " << nom << " perque conte dades no valides.\n";
            continue;
        }
        if (quantitat == 0) {
            ++sense_mesurar;
            continue;
        }
        if (quantitat != EXECUCIONS) {
            std::cerr << "Avis: " << nom << " te " << quantitat << " temps en lloc de "
                      << EXECUCIONS << ".\n";
        }
        mitjanes[n] = std::llround(suma / quantitat);
    }

    if (mitjanes.empty()) {
        std::cerr << "Error: no hi ha dades valides a " << inputs.string()
                  << ". No es modifica la taula.\n";
        return 1;
    }

    fs::path carpeta_sortida = base / "Outputs";
    fs::create_directories(carpeta_sortida);
    fs::path taula = carpeta_sortida / ("dades_" + nom_metode + ".txt");

    std::ofstream sortida(taula);
    sortida << "n temps_ns\n";
    for (const auto& [n, mitjana] : mitjanes) {
        sortida << n << " " << mitjana << "\n";
    }
    sortida.close();

    if (!sortida) {
        std::cerr << "Error: no s'ha pogut escriure " << taula.string() << "\n";
        return 1;
    }
    std::cout << mitjanes.size() << " files desades a " << taula.string() << " ("
              << sense_mesurar << " valors de n encara sense mesurar)\n";
    return 0;
}
