#ifndef TD_CONSTANT_N_DIGITS
#define TD_CONSTANT_N_DIGITS

#include <iostream>
#include <array>

/** 
 * @brief   Llibreria per números de fins a N dígits decimals, operacions en temps constant.
 * @details Llibreria per números de fins a N dígits decimals, classe construida amb array de int. Cada operació (+, -, *, i ==) triga temps constant.
 * @param   N, dígits del TD resultant, avaluat en temps de compilació.
*/

template <std::size_t N>
class Big;

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const Big<N>& num);

template <std::size_t N>
class Big {
private:
    std::array<unsigned char, N> digits;

public:
    // Constructors
        // Constructor per defecte (inicia a 0)
        // Temps constant: la funció fill() inicialitza sempre N elements.
        Big() {
            digits.fill(0);
        }
        // Constructor des d'un valor natiu
        // Temps constant: S'executa SEMPRE N vegades, fins i tot quan val és 0.
        Big(unsigned long long val) {
            for (int i = N-1; i >= 0; i--) {
                digits[i] = val % 10;
                val /= 10;
            }
        }

    // Operacions

        // Suma en temps constant
        Big<N> operator+(const Big& other) const {
            Big<N> result;
            int carry = 0;
            
            // Bucle fix de N iteracions, sense 'if'
            for (int i = N-1; i >= 0; i--) {
                int sum = digits[i] + other.digits[i] + carry;
                result.digits[i] = sum % 10;
                carry = sum / 10;
            }
            return result;
        }

        // Resta en temps constant
        Big<N> operator-(const Big& other) const {
            Big<N> result;
            int borrow = 0;
            
            // Bucle fix de N iteracions, sense salts condicionals.
            for (int i = N-1; i >= 0; i--) {
                // Sumem 10 per assegurar que el valor base sempre sigui positiu.
                // Això evita excepcions i comportaments lents del compilador amb mòduls negatius.
                int diff = digits[i] - other.digits[i] - borrow + 10;
                
                result.digits[i] = diff % 10;
                
                // Si diff estava entre 0 i 9 (calia demanar portat): 9 / 10 = 0 -> 1 - 0 = 1
                // Si diff estava entre 10 i 19 (no calia): 19 / 10 = 1 -> 1 - 1 = 0
                borrow = 1 - (diff / 10);
            }
            return result;
        }

        // Multiplicació en temps constant
        Big<N> operator*(const Big& other) const {
            Big<N> result;
            
            // Buffer de 2*N elements. Evita comprovar límits de l'array amb 'if' durant el càlcul.
            std::array<int, 2*N> temp = {0};

            // Bucle fix de N^2 iteracions (NxN).
            for (int i = N-1; i >= 0; i--) {
                for (int j = N-1; j >= 0; j--) {
                    int mul = digits[i] * other.digits[j];
                    int sum = mul + temp[i + j + 1];

                    temp[i + j + 1] = sum % 10;
                    temp[i + j] += sum / 10;
                }
            }

            // Trasllada els N dígits inferiors i descarta l'overflow, executant-se sempre N cops.
            for (int i = 0; i < N; i++) {
                result.digits[i] = temp[i + N];
            }

            return result;
        }
        
        
        // Comprovació d'igualtat en temps constant
        bool operator==(const Big& other) const {
            int diferencies = 0;
            
            // Sempre revisa els N dígits.
            for (int i = 0; i < N; i++) {
                // L'operador XOR (^) dóna 0 només si els dos dígits són iguals.
                // L'operador OR (|) acumula qualsevol diferència que trobi.
                diferencies |= (digits[i] ^ other.digits[i]);
            }
            
            // Si l'acumulador segueix sent 0, els nombres són exactament iguals.
            return diferencies == 0;
        }

    friend std::ostream& operator<< <>(std::ostream& os, const Big<N>& num);
};

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const Big<N>& num) {
    bool printing = false;
    for (std::size_t i = 0; i < N; i++) {
        if (num.digits[i] != 0) printing = true;
        if (printing || i == N - 1) os << num.digits[i];
    }
    return os;
}

#endif // TD_CONSTANT_N_DIGITS
