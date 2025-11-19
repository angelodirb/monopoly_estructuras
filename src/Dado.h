#ifndef DADO_H_
#define DADO_H_

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// **********************************
// TAD DADO - Generador de numeros aleatorios
// **********************************

struct Dado {
    int valor1;
    int valor2;
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Se crea un dado con valores iniciales 0
     */
    Dado() : valor1(0), valor2(0) {}
};

// **********************************
// OPERACIONES DEL DADO
// **********************************

/**
 * Precondicion: Ninguna
 * Postcondicion: Se generan dos valores aleatorios 1-6 en el dado
 */
void lanzarDado(Dado& dado) {
    dado.valor1 = (rand() % 6) + 1;
    dado.valor2 = (rand() % 6) + 1;
    
    cout << "[DADOS] Resultado de los dados: [" << dado.valor1 << "] + [" 
         << dado.valor2 << "]" << endl;
}

/**
 * Precondicion: El dado debe haber sido lanzado
 * Postcondicion: Retorna la suma de ambos dados
 */
int obtenerSuma(const Dado& dado) {
    return dado.valor1 + dado.valor2;
}

/**
 * Precondicion: El dado debe haber sido lanzado
 * Postcondicion: Retorna true si ambos dados tienen el mismo valor
 */
bool esDoble(const Dado& dado) {
    return dado.valor1 == dado.valor2;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Retorna el valor del primer dado
 */
int obtenerValor1(const Dado& dado) {
    return dado.valor1;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Retorna el valor del segundo dado
 */
int obtenerValor2(const Dado& dado) {
    return dado.valor2;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Muestra informacion del ultimo lanzamiento
 */
void mostrarDado(const Dado& dado) {
    cout << "\n=== INFORMACION DEL DADO ===" << endl;
    cout << "Valor dado 1: " << dado.valor1 << endl;
    cout << "Valor dado 2: " << dado.valor2 << endl;
    cout << "Suma total: " << obtenerSuma(dado) << endl;
    
    if (esDoble(dado)) {
        cout << "[!]DOBLE! Lanzaras de nuevo" << endl;
    } else {
        cout << "Movimiento normal" << endl;
    }
    
    cout << "===========================" << endl;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Reinicia los valores del dado a 0
 */
void reiniciarDado(Dado& dado) {
    dado.valor1 = 0;
    dado.valor2 = 0;
}

/**
 * Precondicion: El dado debe haber sido lanzado
 * Postcondicion: Retorna descripcion de la tirada en string
 */
string obtenerDescripcionLanzamiento(const Dado& dado) {
    string desc = "Tirada: [" + to_string(dado.valor1) + "] + [" + 
                  to_string(dado.valor2) + "] = " + 
                  to_string(obtenerSuma(dado));
    
    if (esDoble(dado)) {
        desc += " (DOBLE)";
    }
    
    return desc;
}

#endif // DADO_H_