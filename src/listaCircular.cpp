#include "listaCircular.h"
#include <iostream>
using namespace std;

ListaCircular crearLista() {
    ListaCircular lista;
    lista.cabeza = nullptr;
    lista.cantidad = 0;
    return lista;
}

bool vaciaLista(const ListaCircular& lista) {
    return lista.cabeza == nullptr;
}

int size(const ListaCircular& lista) {
    return lista.cantidad;
}

void anxLista(ListaCircular& lista, Casilla* nueva) {
    if (!lista.cabeza) {
        lista.cabeza = nueva;
        lista.cabeza->siguiente = lista.cabeza;
    } else {
        Casilla* temp = lista.cabeza;
        while (temp->siguiente != lista.cabeza)
            temp = temp->siguiente;
        temp->siguiente = nueva;
        nueva->siguiente = lista.cabeza;
    }
    lista.cantidad++;
}

void insLista(ListaCircular& lista, Casilla* nueva, int pos) {
    if (pos < 1 || pos > lista.cantidad + 1) {
        cout << "Posición inválida." << endl;
        return;
    }

    if (pos == 1) {
        if (!lista.cabeza) {
            lista.cabeza = nueva;
            lista.cabeza->siguiente = lista.cabeza;
        } else {
            Casilla* ultimo = lista.cabeza;
            while (ultimo->siguiente != lista.cabeza)
                ultimo = ultimo->siguiente;
            nueva->siguiente = lista.cabeza;
            lista.cabeza = nueva;
            ultimo->siguiente = lista.cabeza;
        }
    } else {
        Casilla* temp = lista.cabeza;
        for (int i = 1; i < pos - 1; i++)
            temp = temp->siguiente;
        nueva->siguiente = temp->siguiente;
        temp->siguiente = nueva;
    }
    lista.cantidad++;
}

void elimLista(ListaCircular& lista, int pos) {
    if (pos < 1 || pos > lista.cantidad) {
        cout << "Posición inválida." << endl;
        return;
    }

    if (pos == 1) {
        Casilla* temp = lista.cabeza;
        if (lista.cantidad == 1) {
            lista.cabeza = nullptr;
        } else {
            Casilla* ultimo = lista.cabeza;
            while (ultimo->siguiente != lista.cabeza)
                ultimo = ultimo->siguiente;
            lista.cabeza = lista.cabeza->siguiente;
            ultimo->siguiente = lista.cabeza;
        }
        delete temp;
    } else {
        Casilla* temp = lista.cabeza;
        for (int i = 1; i < pos - 1; i++)
            temp = temp->siguiente;
        Casilla* eliminar = temp->siguiente;
        temp->siguiente = eliminar->siguiente;
        delete eliminar;
    }
    lista.cantidad--;
}

void mostrarLista(const ListaCircular& lista) {
    if (!lista.cabeza) {
        cout << "(Lista vacía)" << endl;
        return;
    }
    Casilla* temp = lista.cabeza;
    do {
        cout << "- " << temp->getNombre() << endl;
        temp = temp->siguiente;
    } while (temp != lista.cabeza);
}

Casilla* getCabeza(const ListaCircular& lista) {
    return lista.cabeza;
}