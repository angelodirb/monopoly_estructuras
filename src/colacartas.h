#ifndef COLACARTAS_H_
#define COLACARTAS_H_

#include <iostream>
#include <stdexcept>
#include <string>
#include "cola.h"
#include "cartas.h"
using namespace std;

// ===========================
// TAD ColaDeCartas
// ===========================
//
// Representa un mazo de Monopoly (Suerte o Arca Comunal).
// Las cartas se almacenan en una cola FIFO y todas son del mismo tipo.
//
// Invariante de representación:
//  Para toda carta c en cartas, c.tipo == tipoMazo
//
// ===========================

struct ColaDeCartas {
    string tipoMazo;     // "Suerte" o "Arca Comunal"
    Cola<Carta> cartas;  // Cola base
};

// crearColaDeCartas: string -> ColaDeCartas
// PRE: tipoMazo == "Suerte" o tipoMazo == "Arca Comunal".
// POST: Devuelve una cola de cartas vacía de ese tipo.
ColaDeCartas crearColaDeCartas(const string& tipoMazo) {
    ColaDeCartas mazo;
    mazo.tipoMazo = tipoMazo;
    mazo.cartas = crearCola<Carta>();
    return mazo;
}

// encolarCarta: ColaDeCartas x Carta -> ColaDeCartas
// PRE: carta.tipo == mazo.tipoMazo.
// POST: Devuelve el mazo con la carta agregada al final.
ColaDeCartas encolarCarta(ColaDeCartas mazo, const Carta& carta) {
    if (carta.tipo != mazo.tipoMazo)
        throw invalid_argument("encolarCarta: tipo de carta incompatible con el mazo");
    mazo.cartas = anxCola(mazo.cartas, carta);
    return mazo;
}

// sacarCarta: ColaDeCartas -> ColaDeCartas
// PRE: La cola no está vacía.
// POST: Elimina la carta del frente del mazo.
ColaDeCartas sacarCarta(ColaDeCartas mazo) {
    if (vaciaCola(mazo.cartas))
        throw out_of_range("sacarCarta: la cola está vacía");
    mazo.cartas = elimCola(mazo.cartas);
    return mazo;
}

// verCartaFrente: ColaDeCartas -> Carta
// PRE: La cola no está vacía.
// POST: Retorna la carta al frente sin modificar la cola.
Carta verCartaFrente(const ColaDeCartas& mazo) {
    if (vaciaCola(mazo.cartas))
        throw out_of_range("verCartaFrente: la cola está vacía");
    return infoCola(mazo.cartas);
}

// vaciaColaDeCartas: ColaDeCartas -> bool
// PRE: ---
// POST: Retorna true si el mazo no contiene cartas.
bool vaciaColaDeCartas(const ColaDeCartas& mazo) {
    return vaciaCola(mazo.cartas);
}

#endif
