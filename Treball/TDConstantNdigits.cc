#include "TDConstantNdigits.hh"

// Constructor per defecte (inicia a 0)
// Temps constant: la funció fill() inicialitza sempre N elements.
Big::Big() {
    digits.fill(0);
}

// Constructor des d'un valor
// Temps constant: S'executa SEMPRE N vegades, fins i tot quan val és 0.
Big::Big(unsigned long long val) {
    for (int i = Big::N-1; i >= 0; i--) {
        digits[i] = val % 10;
        val /= 10;
    }
}

// Suma en temps constant
Big Big::operator+(const Big& other) const {
    Big result;
    int carry = 0;
    
    // Bucle fix de N iteracions, sense 'if'
    for (int i = Big::N-1; i >= 0; i--) {
        int sum = digits[i] + other.digits[i] + carry;
        result.digits[i] = sum % 10;
        carry = sum / 10;
    }
    return result;
}

// Resta en temps constant
Big Big::operator-(const Big& other) const {
    Big result;
    int borrow = 0;
    
    // Bucle fix de N iteracions, sense salts condicionals.
    for (int i = Big::N-1; i >= 0; i--) {
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
Big Big::operator*(const Big& other) const {
    Big result;
    
    // Buffer de 80 elements. Evita comprovar límits de l'array amb 'if' durant el càlcul.
    std::array<int, 80> temp = {0};

    // Bucle fix de N^2 iteracions (NxN).
    for (int i = Big::N-1; i >= 0; i--) {
        for (int j = Big::N-1; j >= 0; j--) {
            int mul = digits[i] * other.digits[j];
            int sum = mul + temp[i + j + 1];

            temp[i + j + 1] = sum % 10;
            temp[i + j] += sum / 10;
        }
    }

    // Trasllada els N dígits inferiors i descarta l'overflow, executant-se sempre N cops.
    for (int i = 0; i < Big::N; i++) {
        result.digits[i] = temp[i + Big::N];
    }

    return result;
}

// Operador d'escriptura
std::ostream& operator<<(std::ostream& os, const Big& num) {
    bool printing = false;
    for (int i = 0; i < Big::N; i++) {
        if (num.digits[i] != 0) printing = true;
        // Imprimeix el dígit si ja hem començat a imprimir, o si és l'últim dígit (per mostrar "0")
        if (printing || i == Big::N-1) os << num.digits[i];
    }
    return os;
}


// Comprovació d'igualtat en temps constant
bool Big::operator==(const Big& other) const {
    int diferencies = 0;
    
    // Sempre revisa els N dígits.
    for (int i = 0; i < Big::N; i++) {
        // L'operador XOR (^) dóna 0 només si els dos dígits són iguals.
        // L'operador OR (|) acumula qualsevol diferència que trobi.
        diferencies |= (digits[i] ^ other.digits[i]);
    }
    
    // Si l'acumulador segueix sent 0, els nombres són exactament iguals.
    return diferencies == 0;
}