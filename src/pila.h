#ifndef PILA_H_
#define PILA_H_

#include "lista.h"

template <typename T>
using Pila = Nodo<T>*;


// crearPila: -> Pila
template <typename T>
Pila<T> crearPila() {
    Pila<T> p = crearLista<T>();
    return p;
}

// anxPila: Pila * T -> Pila
template <typename T>
Pila<T> anxPila(Pila<T> p, const T e) {
    if (p == NULL) {
        return anxLista(p, e);
    }
    else {
        return insLista(p, e, 1);
    }
}

// infoPila: Pila -> T
template <typename T>
T infoPila(Pila<T> p) {
    return infoLista(p, 1);
}

// elimPila: Pila -> Pila
template <typename T>
Pila<T> elimPila(Pila<T> p) {
    return elimLista(p, 1);
}

// vaciaPila: Pila -> bool
template <typename T>
bool vaciaPila(Pila<T> p) {
    return vaciaLista(p);
}

#endif