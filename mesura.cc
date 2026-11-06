// Plantilla de mesura: executa un algorisme 10 vegades amb la n indicada i
// desa cada temps a FormatDades/metodeM/Inputs/dades_nN.txt, en format llegible per R:
//     n temps_ns
//     10 79
//
// Execucio (des de l'arrel del repositori):  make run P=mesura
// Es demana el metode (1, 2 o 3) i la n. La n no es valida.
// Si el fitxer ja existeix, se sobreescriu.

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

const int EXECUCIONS = 10;

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

// Les variables volatile eviten que el compilador (-O2) elimini el calcul
// o el mogui fora de la zona cronometrada.
template <typename Algorisme>
long long mesura_ns(Algorisme algorisme, long long n) {
    volatile long long entrada = n;
    volatile unsigned long long sortida = 0;

    auto inici = std::chrono::steady_clock::now();
    sortida = algorisme(entrada);
    auto fi = std::chrono::steady_clock::now();

    (void)sortida;
    return std::chrono::duration_cast<std::chrono::nanoseconds>(fi - inici).count();
}

int main() {
    if (!fs::is_directory("FormatDades")) {
        std::cerr << "Error: no es troba FormatDades. Executa des de l'arrel del repositori.\n";
        return 1;
    }

    int metode;
    long long n;
    std::cout << "Metode (1, 2 o 3): ";
    if (!(std::cin >> metode) || metode < 1 || metode > 3) {
        std::cerr << "Error: el metode ha de ser 1, 2 o 3.\n";
        return 1;
    }
    std::cout << "n: ";
    if (!(std::cin >> n)) {
        std::cerr << "Error: n ha de ser un nombre enter.\n";
        return 1;
    }

    // Primer es mesura i despres s'escriu, perque l'escriptura no afecti els temps
    // i una execucio interrompuda no deixi un fitxer a mitges.
    std::array<long long, EXECUCIONS> temps;
    for (long long& t : temps) {
        if (metode == 1) t = mesura_ns(metode1, n);
        else if (metode == 2) t = mesura_ns(metode2, n);
        else t = mesura_ns(metode3, n);
    }

    fs::path carpeta = fs::path("FormatDades") / ("metode" + std::to_string(metode)) / "Inputs";
    fs::create_directories(carpeta);
    fs::path fitxer = carpeta / ("dades_n" + std::to_string(n) + ".txt");

    std::ofstream sortida(fitxer);
    sortida << "n temps_ns\n";
    for (long long t : temps) sortida << n << " " << t << "\n";
    sortida.close();

    if (!sortida) {
        std::cerr << "Error: no s'ha pogut escriure " << fitxer.string() << "\n";
        return 1;
    }
    std::cout << EXECUCIONS << " temps (ns) desats a " << fitxer.string() << "\n";
    return 0;
}
