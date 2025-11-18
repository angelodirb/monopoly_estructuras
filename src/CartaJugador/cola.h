#ifndef COLA_H_
#define COLA_H_

#include "lista.h"

template <typename T>
using Cola = Nodo<T>*;


// crearCola: -> Cola
template <typename T>
Cola<T> crearCola() {
    Cola<T> p = crearLista<T>();
    return p;
}

// anxCola: Cola * T -> Cola
template <typename T>
Cola<T> anxCola(Cola<T> p, const T e) {
    return anxLista(p, e);
}

// infoCola: Cola -> T
template <typename T>
T infoCola(Cola<T> p) {
    return infoLista(p, 1);
}

// elimCola: Cola -> Cola
template <typename T>
Cola<T> elimCola(Cola<T> p) {
    return elimLista(p, 1);
}

// vaciaCola: Cola -> bool
template <typename T>
bool vaciaCola(Cola<T> p) {
    return vaciaLista(p);
}

#endif