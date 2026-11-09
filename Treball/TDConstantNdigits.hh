#ifndef TD_CONSTANT_N_DIGITS
#define TD_CONSTANT_N_DIGITS

#include <iostream>
#include <array>

/** 
 * @brief   Llibreria per números de fins a 40 dígits decimals, operacions en temps constant.
 * @details Llibreria per números de fins a 40 dígits decimals, classe construida amb array de int. Cada operació (+, -, *, i ==) triga temps constant.
 * @param   N, dígits del TD resultant, avaluat en temps de compilació.
*/

class Big {
private:
    static constexpr int N = 40;
    std::array<unsigned char, N> digits;

public:
    // Constructors
    Big();
    Big(unsigned long long val);

    // Operacions
    Big operator+(const Big& other) const;
    Big operator-(const Big& other) const;
    Big operator*(const Big& other) const;
    bool operator==(const Big& other) const;

    // Sobrecàrrega per imprimir
    friend std::ostream& operator<<(std::ostream& os, const Big& num);
};

#endif // TD_CONSTANT_N_DIGITS