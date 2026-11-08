#ifndef INT_40_DIGITS_TEMPS_FIXE
#define INT_40_DIGITS_TEMPS_FIXE

#include <iostream>
#include <array>

/** 
 * @brief   Llibreria per números de fins a 40 dígits decimals, operacions en temps constant.
 * @details Llibreria per números de fins a 40 dígits decimals, classe construida amb array de int. Cada operació (+, -, *, i ==) triga temps constant.
*/

class Int40TempsFixe {
private:
    std::array<int, 40> digits;

public:
    // Constructors
    Int40TempsFixe();
    Int40TempsFixe(unsigned long long val);

    // Operacions
    Int40TempsFixe operator+(const Int40TempsFixe& other) const;
    Int40TempsFixe operator-(const Int40TempsFixe& other) const;
    Int40TempsFixe operator*(const Int40TempsFixe& other) const;
    bool operator==(const Int40TempsFixe& other) const;

    // Sobrecàrrega per imprimir
    friend std::ostream& operator<<(std::ostream& os, const Int40TempsFixe& num);
};

#endif // INT_40_DIGITS_TEMPS_FIXE