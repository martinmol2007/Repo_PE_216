#include <iostream>
#include <vector>

// Calcula el n-èssim nombre de Fibonacci fent servir programació dinàmica i un vector auxiliar
unsigned long long fibonacci_dp(long long n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    // Vector auxiliar de mida n + 1
    std::vector<unsigned long long> f(n + 1);

    f[0] = 0;
    f[1] = 1;

    for (long long i = 2; i <= n; ++i) {
        f[i] = f[i - 1] + f[i - 2];
    }

    return f[n];
}

int main() {
    long long n;

    std::cout << "long longrodueix un nombre enter (n): ";
    std::cin >> n;

    // Validació per evitar nombres negatius
    if (n < 0) {
        std::cout << "Error: El nombre ha de ser major o igual a 0." << std::endl;
        return 1;
    }

/*   // Alerta de desbordament (overflow) si n és superior a 93
    if (n > 93) {
        std::cout << "Avís: Els resultats per a n > 93 patiran un desbordament amb 'unsigned long long'." << std::endl;
    }*/ 

    unsigned long long resultat = fibonacci_dp(n);
    std::cout << "F(" << n << ") = " << resultat << std::endl;

    return 0;
}
//      1000
//      10000
//      100000
//      1000000
//      10000000
//      100000000