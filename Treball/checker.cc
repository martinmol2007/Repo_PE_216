#include <iostream>
#include <vector>
#include <cassert>
#include "Int40DigitsTempsFixe.hh"
#include <utility>
using namespace std;

/**
 * @brief Comprova que la funció donada retorna valors correctes per a tots els termes dins de l'interval.
 */


//Genera un vector amb els valors pels termes de fibonacci de 0 a 193
//Generat amb memoitzacio simple.
vector<Int40TempsFixe> generarFibonacci193() {
    vector<Int40TempsFixe> fibSeq;
    fibSeq.reserve(193);
    fibSeq.push_back(Int40TempsFixe(1ULL));
    fibSeq.push_back(Int40TempsFixe(1ULL));
    for (int i = 2; i < 193; i++) {
        fibSeq.push_back(fibSeq[i - 1] + fibSeq[i - 2]);
    }
    return fibSeq;
}

//Retorna vector amb els valors, generat una única vegada
const vector<Int40TempsFixe>& obtenirTaulaFibonacci() {
    static std::vector<Int40TempsFixe> taula_guardada = generarFibonacci193();
    return taula_guardada;
}

/*
INSERTAR FUNCIÓ AQUÍ
*/

/*
INSERTAR FUNCIÓ AQUÍ
*/


/**
 * @brief Funció màscara, cridar a la funció a comprovar
 * @param n és el terme actual, és un int, si és necessari fer un canvi de tipus per la funció a validar s'ha de fer en la crida
 */
Int40TempsFixe funcioMascara(int n) {
    //Crida a funció que volem validar
    //... miFunc(n);
}


int main() {
    const std::vector<Int40TempsFixe>& valors_reals = obtenirTaulaFibonacci();

    std::cout << "Comprovant Fast Doubling...\n";
    
    for (int i = 1; i <= 193; i++) {
        Int40TempsFixe valorCalculat = funcioMascara(i);

        //Comprovar que el valor calculat pel terme actual correspon al de la taula
        assert(valorCalculat == valors_reals[i - 1]); 
    }

    std::cout << "Tots els termes son correctes\n";
    return 0;
}