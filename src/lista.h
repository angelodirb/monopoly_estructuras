#ifndef LISTA_H_
#define LISTA_H_

#include <iostream>
#include <stdexcept>

template <typename T>
struct Nodo {
    T dato;
    int prioridad;
    Nodo<T>* sig;
    Nodo<T>* ant;
};

template <typename T>
using Lista = Nodo<T>*;

// crearLista: -> Lista
template <typename T>
Lista<T> crearLista() {
    return NULL;
}

// vaciaLista: Lista -> bool
template <typename T>
bool vaciaLista(const Lista<T> lst) {
    return lst == NULL;
}

// longLista: Lista -> int
template <typename T>
int longLista(const Lista<T> lst) {
    int n = 0;
    Lista<T> tmp = lst;
    while (tmp != NULL) {
        n++;
        tmp = tmp->sig;
    }
    return n;
}

// anxLista: Lista x T -> Lista
template <typename T>
Lista<T> anxLista(Lista<T> lst, const T e) {
    Lista<T> nuevo = (Lista<T>)malloc(sizeof(Nodo<T>));
    nuevo->dato = e;
    nuevo->prioridad = 0; // no aplica
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if (lst == NULL) {
        return nuevo;
    } else {
        Lista<T> tmp = lst;
        while (tmp->sig != NULL)
            tmp = tmp->sig;
        tmp->sig = nuevo;
        nuevo->ant = tmp;
        return lst;
    }
}

// infoLista: Lista x int -> T
template <typename T>
T infoLista(const Lista<T> lst, int pos) {
    if (pos < 1 || pos > longLista(lst))
        throw std::out_of_range("infoLista: Posicion invalida");

    Lista<T> tmp = lst;
    for (int i = 1; i < pos; i++)
        tmp = tmp->sig;
    return tmp->dato;
}

// elimLista: Lista x int -> Lista
template <typename T>
Lista<T> elimLista(Lista<T> lst, int pos) {
    if (pos < 1 || pos > longLista(lst))
        throw std::out_of_range("elimLista: No existe esa posicion");

    if (pos == 1) {
        Lista<T> tmp = lst;
        lst = lst->sig;
        if (lst != NULL) lst->ant = NULL;
        tmp->sig = NULL;
        free(tmp);
    } else {
        Lista<T> tmp = lst;
        for (int i = 1; i < pos - 1; i++)
            tmp = tmp->sig;
        Lista<T> tmp2 = tmp->sig;
        tmp->sig = tmp->sig->sig;
        if (tmp->sig != NULL)
            tmp->sig->ant = tmp;
        tmp2->sig = NULL;
        tmp2->ant = NULL;
        free(tmp2);
    }
    return lst;
}

#endif