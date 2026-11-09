#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>
#include <string>
using namespace std;
#include "TDConstantNdigits.hh"
#include <utility>
#include <unordered_map>



/**
 * @brief Aquest programa processa valors del fitxer @param fitxerEntrada opera amb 2 algorismes, fa @param M execucions
 *        calcula la mitjana del temps d'execució per cada algorisme i envia el resultat organitzat a @param fitxerSortida
 * 
 * @param N digits del terme més a considerar. Per Fibonacci, Dígits = floor(n * log(phi)) + 1
 * @param fitxerEntrada arxiu on estan els valors a processar
 * @param M execucions a realitzar de cada algorisme
 * @param fitxerSortida arxiu on escriu els resultats ordenats.
 *        Conté: El valor entrat, l'ordre aleatori de les execucions (per replicabilitat), i les mitjanes de cada algorisme
 * @param afegirOrdre booleà, si cert afegir ordre a arxiu sortida, altrament no afegir
 */

 //
//////  Variables globals
//
const int N = 100;
using BigVal = Big<N>;

const int M = 50;
string fitxerEntrada = "valorsAleatoris.txt";
string fitxerSortida = "resultatsMeus.txt";
bool afegirOrdre = false;


void debug(int& n){
    cout << "ARRIBA AQUÍ" << n++ << endl;
}
int constantDebug=1;


//
//////  Funcions dels algorismes a comparar
//

pair<BigVal, BigVal> fibFastDoubling(int k) {
    if (k == 0) return {BigVal(0), BigVal(1)};
    
    // Recursió per F(k/2) i F(k/2 + 1)
    pair<BigVal, BigVal> p = fibFastDoubling(k >> 1);
    BigVal a = p.first;   // F(k)
    BigVal b = p.second;  // F(k+1)
    
    // c = F(2k) = a * (2*b - a)
    // b+b en lloc de 2*b ja que classe no té multiplicació per literal
    BigVal c = a * ((b + b) - a);
    
    // d = F(2k+1) = a^2 + b^2
    BigVal d = (a * a) + (b * b);
    
    // Valors a retornar segons si k és senar o parell 
    if (k & 1)  return {d, c + d};  // F(2k+1), F(2k+2)
    else        return {c, d};      // F(2k), F(2k+1)
}
BigVal fibMemoization(int n, unordered_map<int, BigVal>& memo) {
    if(n < 2) return BigVal(n);

    //Cercar terme a registre. Si està, enviar-lo com a resposta
    auto it = memo.find(n);
    if(it != memo.end()) return it->second;
    //Si no està, calcular-lo recursivament i guardar al registre
    memo[n] = fibMemoization(n-1, memo) + fibMemoization(n-2, memo);
    return memo[n];
}

//
//////  Punters a funcions a comparar
//

// {retorn} (*punterFuncio)({paràmetres}) = funcioReal;

pair<BigVal, BigVal> (*algorismeComparat1)(int k) = fibFastDoubling;
BigVal (*algorismeComparat2)(int n, unordered_map<int, BigVal>& memo) = fibMemoization;

//
//////  Noms algorismes a comparar
//

string nomAlg1 = "Memoització";
string nomAlg2 = "Duplicació Ràpida";







/*
// Funció auxiliar, calcula la mediana d'un vector de temps
double calcular_mediana(vector<double>& temps) {
    if (temps.empty()) return 0.0;
    sort(temps.begin(), temps.end());
    size_t n = temps.size();
    if (n % 2 == 0) {
        return (temps[n / 2 - 1] + temps[n / 2]) / 2.0;
    } else {
        return temps[n / 2];
    }
}
*/


int main() {
    auto t0 = chrono::high_resolution_clock::now();
    // Configurar generador de nombres aleatoris per a l'ordre d'execució
    random_device rd;
    mt19937 gen(rd());

    // Obrir fitxers
    ifstream arxiu_entrada(fitxerEntrada);
    ofstream arxiu_sortida(fitxerSortida);

    // Comprovació d'errors en obrir els fitxers
    if (!arxiu_entrada.is_open()) {
        cerr << "Error: No s'ha pogut obrir el fitxer d'entrada: " << fitxerEntrada << "\n";
        return 1;
    }
    if (!arxiu_sortida.is_open()) {
        cerr << "Error: No s'ha pogut crear el fitxer de sortida: " << fitxerSortida << "\n";
        return 1;
    }
    // Escriure la capçalera al fitxer de sortida
    arxiu_sortida << " Valor\t\tMitjana_" + nomAlg1 + "(μs)\t\tMitjana_"+ nomAlg2 + "(μs)";
    string separador;
    if(afegirOrdre) {
        arxiu_sortida <<  "\t\tOrdre_Execucio \n";
        separador = string(4*M-1 + 56+30, '#');
    }
    else {
        arxiu_sortida << "\n";
        separador = string(76, '#');
    }

    arxiu_sortida << separador + "\n" + separador + "\n" + "\n" + "\n";

    int valor_actual;
    // Llegir autònomament fins al final del fitxer
    while (arxiu_entrada >> valor_actual) {
        // Definim l'ordre d'execució i el barregem aleatòriament
        vector<int> ordre_execucio(M*2);                                //Crear valor amb totes les execucions
        for(auto i = 0; i < 2*M; i++) ordre_execucio[i] = i%2;          //Omplir de 0 i 1
        shuffle(ordre_execucio.begin(), ordre_execucio.end(), gen);     //Distribució aleatòriadel moviment

        // Càlcul de les mitjanes
        double mitjana_f1 = 0;
        double mitjana_f2 = 0;
        // Executem en l'ordre aleatori generat
        for (int func_id : ordre_execucio) {
            if (func_id == 0) {
                //Variables addicionals algorisme1:

                auto inici = chrono::high_resolution_clock::now();
                algorismeComparat1(valor_actual);
                auto fi = chrono::high_resolution_clock::now();
                
                chrono::duration<double, micro> duracio = fi - inici;
                mitjana_f1 += duracio.count();
            } 
            else if (func_id == 1) {
                //Variables addicionals algorisme2:
                unordered_map<int, BigVal> memo;

                auto inici = chrono::high_resolution_clock::now();
                algorismeComparat2(valor_actual, memo);
                auto fi = chrono::high_resolution_clock::now();
                
                chrono::duration<double, micro> duracio = fi - inici;
                mitjana_f2 += duracio.count();
            }
        }
        
        mitjana_f1 /= M;
        mitjana_f2 /= M;

        string text_ordre;
        if(afegirOrdre) {
            // Donar format a l'ordre d'execució (ex: "0,1" o "1,0")
            for(auto i = 0; i < 2*M; i++) {
                text_ordre += to_string(ordre_execucio[i]);
                if (i < 2*M - 1) text_ordre += ",";
            }
        }

        // Escriure la línia de resultats al fitxer de sortida
        arxiu_sortida << valor_actual << "\t\t" 
                      << mitjana_f1 << "\t\t\t\t" 
                      << mitjana_f2;
        if(afegirOrdre) arxiu_sortida << "\t\t\t\t\t" << text_ordre << "\n";
        else arxiu_sortida << "\n";
    }
    arxiu_entrada.close();
    arxiu_sortida.close();

    std::cout << "Execució finalitzada. Resultats desats a: " << fitxerSortida << "\n";

    auto t1 = chrono::high_resolution_clock::now();
    chrono::duration<double, micro> t = t1-t0;
    cout << "Temps total per fer tot " << t.count() / 1000000 << " segons" << endl; 
    
    return 0;
}