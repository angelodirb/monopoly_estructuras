#ifndef LISTACIRCULAR_H_
#define LISTACIRCULAR_H_

#include "casilla/casilla.h"
#include <iostream>

struct ListaCircular {
    Casilla* cabeza;
    int cantidad;
};

// Solo declaraciones de funciones (implementaciones en listaCircular.cpp)
ListaCircular crearLista();
bool vaciaLista(const ListaCircular& lista);
int size(const ListaCircular& lista);
void anxLista(ListaCircular& lista, Casilla* nueva);
void insLista(ListaCircular& lista, Casilla* nueva, int pos);
void elimLista(ListaCircular& lista, int pos);
void mostrarLista(const ListaCircular& lista);
Casilla* getCabeza(const ListaCircular& lista);

#endif // LISTACIRCULAR_H_