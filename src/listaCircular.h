#ifndef LISTACIRCULAR_H_
#define LISTACIRCULAR_H_

#include <iostream>
#include <stdexcept>
#include <cstdlib>
using namespace std;

template<typename T>
struct Nodo {
    T dato;
    Nodo<T>* sig;
};

template<typename T>
using Lista = Nodo<T>*;

template<typename T>
Lista<T> crearLista() {
    return NULL;
}

template<typename T>
Lista<T> anxLista(Lista<T> lst, T e) {
    Lista<T> nuevo = (Lista<T>)malloc(sizeof(Nodo<T>));
    nuevo->dato = e;

    if (lst == NULL) {
        nuevo->sig = nuevo;
        return nuevo;
    }

    Lista<T> ptr = lst;
    while (ptr->sig != lst) {
        ptr = ptr->sig;
    }
    ptr->sig = nuevo;
    nuevo->sig = lst;
    return lst;
}

template<typename T>
int size(Lista<T> lst) {
    if (lst == NULL) return 0;
    int n = 0;
    Lista<T> ptr = lst;
    do {
        n++;
        ptr = ptr->sig;
    } while (ptr != lst);
    return n;
}

template<typename T>
Lista<T> insLista(Lista<T> lst, int pos, T e) {
    if (pos < 0 || pos > size(lst)) {
        throw out_of_range("FUERA DE RANGO");
    }

    Lista<T> nuevo = (Lista<T>)malloc(sizeof(Nodo<T>));
    nuevo->dato = e;

    if (lst == NULL) { // lista vacía
        nuevo->sig = nuevo;
        return nuevo;
    }

    if (pos == 0) {
        Lista<T> ptr = lst;
        while (ptr->sig != lst) {
            ptr = ptr->sig;
        }
        ptr->sig = nuevo;
        nuevo->sig = lst;
        return nuevo; // nuevo pasa a ser la cabeza
    }

    Lista<T> tmp = lst;
    int i = 0;
    while (tmp->sig != lst && i < pos - 1) {
        tmp = tmp->sig;
        i++;
    }

    nuevo->sig = tmp->sig;
    tmp->sig = nuevo;
    return lst;
}

template<typename T>
Lista<T> elimLista(Lista<T> lst, int pos) {
    if (lst == NULL) return lst;

    if (pos < 0 || pos >= size(lst))
        throw out_of_range("FUERA DE RANGO");

    if (pos == 0) {
        if (lst->sig == lst) { // solo un nodo
            free(lst);
            return NULL;
        }
        Lista<T> ult = lst;
        while (ult->sig != lst) {
            ult = ult->sig;
        }
        ult->sig = lst->sig;
        Lista<T> nuevoInicio = lst->sig;
        free(lst);
        return nuevoInicio;
    }

    Lista<T> tmp = lst;
    Lista<T> prev = NULL;
    int i = 0;
    while (tmp->sig != lst && i < pos) {
        prev = tmp;
        tmp = tmp->sig;
        i++;
    }

    prev->sig = tmp->sig;
    free(tmp);
    return lst;
}

template<typename T>
void mostrarLista(Lista<T> lst) {
    if (lst == NULL) return;
    Lista<T> tmp = lst;
    do {
        cout << tmp->dato << " ";
        tmp = tmp->sig;
    } while (tmp != lst);
    cout << "\n";
}

#endif
