#include "Int40DigitsTempsFixe.hh"

// Constructor per defecte (inicia a 0)
// Temps constant: la funció fill() inicialitza sempre 40 elements.
Int40TempsFixe::Int40TempsFixe() {
    digits.fill(0);
}

// Constructor des d'un enter natiu
// Temps constant: S'executa SEMPRE 40 vegades, fins i tot quan val és 0.
Int40TempsFixe::Int40TempsFixe(unsigned long long val) {
    for (int i = 39; i >= 0; i--) {
        digits[i] = val % 10;
        val /= 10;
    }
}

// Suma en temps constant
Int40TempsFixe Int40TempsFixe::operator+(const Int40TempsFixe& other) const {
    Int40TempsFixe result;
    int carry = 0;
    
    // Bucle fix de 40 iteracions, sense 'if'
    for (int i = 39; i >= 0; i--) {
        int sum = digits[i] + other.digits[i] + carry;
        result.digits[i] = sum % 10;
        carry = sum / 10;
    }
    return result;
}

// Resta en temps constant
Int40TempsFixe Int40TempsFixe::operator-(const Int40TempsFixe& other) const {
    Int40TempsFixe result;
    int borrow = 0;
    
    // Bucle fix de 40 iteracions, sense salts condicionals.
    for (int i = 39; i >= 0; i--) {
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
Int40TempsFixe Int40TempsFixe::operator*(const Int40TempsFixe& other) const {
    Int40TempsFixe result;
    
    // Buffer de 80 elements. Evita comprovar límits de l'array amb 'if' durant el càlcul.
    std::array<int, 80> temp = {0};

    // Bucle fix de 1.600 iteracions (40x40).
    for (int i = 39; i >= 0; i--) {
        for (int j = 39; j >= 0; j--) {
            int mul = digits[i] * other.digits[j];
            int sum = mul + temp[i + j + 1];

            temp[i + j + 1] = sum % 10;
            temp[i + j] += sum / 10;
        }
    }

    // Trasllada els 40 dígits inferiors i descarta l'overflow, executant-se sempre 40 cops.
    for (int i = 0; i < 40; i++) {
        result.digits[i] = temp[i + 40];
    }

    return result;
}

// Operador d'escriptura
std::ostream& operator<<(std::ostream& os, const Int40TempsFixe& num) {
    bool printing = false;
    for (int i = 0; i < 40; i++) {
        if (num.digits[i] != 0) printing = true;
        // Imprimeix el dígit si ja hem començat a imprimir, o si és l'últim dígit (per mostrar "0")
        if (printing || i == 39) os << num.digits[i];
    }
    return os;
}


// Comprovació d'igualtat en temps constant
bool Int40TempsFixe::operator==(const Int40TempsFixe& other) const {
    int diferencies = 0;
    
    // Sempre revisa els 40 dígits.
    for (int i = 0; i < 40; i++) {
        // L'operador XOR (^) dóna 0 només si els dos dígits són iguals.
        // L'operador OR (|) acumula qualsevol diferència que trobi.
        diferencies |= (digits[i] ^ other.digits[i]);
    }
    
    // Si l'acumulador segueix sent 0, els nombres són exactament iguals.
    return diferencies == 0;
}
