#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include "TDConstantNdigits.hh"
using namespace std;

/**
 * @brief   Algorismes per calcular n-èssim terme de la seqüència de fibonacci
 * 
 * @details FastDoubling utilitza una manipulació de l'expressió del Fibonacci F(2k) i F(2k+1),
 *          mètode semblant a exponenciació ràpida.
 *          Memoization guarda càlculs anteriors a un map, així només calcularà cada terme una única vegada.
 * 
 *          FastDoubling té cost asimptòtic O(log(n)).
 *          Memoization té cost asimptòtic O(n).
 */


pair<Big, Big> fibFastDoubling(int k) {
    if (k == 0) return {Big(0), Big(1)};
    
    // Recursió per F(k/2) i F(k/2 + 1)
    pair<Big, Big> p = fibFastDoubling(k >> 1);
    Big a = p.first;   // F(k)
    Big b = p.second;  // F(k+1)
    
    // c = F(2k) = a * (2*b - a)
    // b+b en lloc de 2*b ja que classe no té multiplicació per literal
    Big c = a * ((b + b) - a);
    
    // d = F(2k+1) = a^2 + b^2
    Big d = (a * a) + (b * b);
    
    // Valors a retornar segons si k és senar o parell 
    if (k & 1)  return {d, c + d};  // F(2k+1), F(2k+2)
    else        return {c, d};      // F(2k), F(2k+1)
}


Big fibMemoization(int n, unordered_map<int, Big>& memo) {
    if(n < 2) return Big(n);

    //Cercar terme a registre. Si està, enviar-lo com a resposta
    auto it = memo.find(n);
    if(it != memo.end()) return it->second;
    //Si no està, calcular-lo recursivament i guardar al registre
    memo[n] = fibMemoization(n-1, memo) + fibMemoization(n-2, memo);
    return memo[n];
}