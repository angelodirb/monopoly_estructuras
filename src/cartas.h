#ifndef CARTAS_H_
#define CARTAS_H_

#include <string>
#include <iostream>
using namespace std;

// ===========================
// TAD Carta
// ===========================
//
// Representa una carta de Monopoly, que puede pertenecer al mazo de "Suerte"
// o "Arca Comunal". Cada carta tiene un texto descriptivo y un efecto numérico.
//
// Invariante de representación:
//  tipo == "Suerte" o tipo == "Arca Comunal"
//
// ===========================

struct Carta {
    string tipo;    // "Suerte" o "Arca Comunal"
    string texto;   // Texto descriptivo de la carta
    int efecto;     // Valor del efecto (ej: +200 o -50)
};

// crearCarta: string x string x int -> Carta
// PRE: tipo == "Suerte" o tipo == "Arca Comunal".
// POST: Retorna una carta con los datos indicados.
Carta crearCarta(const string& tipo, const string& texto, int efecto) {
    Carta c;
    c.tipo = tipo;
    c.texto = texto;
    c.efecto = efecto;
    return c;
}

// mostrarCarta: Carta -> void
// PRE: ---
// POST: Muestra el contenido de la carta en pantalla.
void mostrarCarta(const Carta& c) {
    cout << "[Tipo: " << c.tipo << "] "
         << c.texto << " (Efecto: " << c.efecto << ")" << endl;
}

#endif
