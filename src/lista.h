#ifndef LISTA_H_
#define LISTA_H_

#include <cstdlib>

template <typename T>
struct Nodo
{
    T dato;
    struct Nodo *sig;
    struct Nodo *ant;
};
template <typename T>
using Lista = Nodo<T>*;


//crearLista: -> Lista
template <typename T>
Lista<T> crearLista()
{
    Lista<T> lst;
    lst = NULL;
    return lst;
}

//anxLista: Lista x T -> Lista
template <typename T>
Lista<T> anxLista(Lista<T> lst, T e)
{
    Lista<T> nuevo = (Lista<T>)malloc(sizeof(Nodo<T>));
    nuevo->dato = e;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if(lst == NULL)
        lst = nuevo;
    else
    {
        Lista<T> tmp = lst;
        while(tmp->sig != NULL)
            tmp = tmp->sig;
        tmp->sig = nuevo;
        nuevo->ant = tmp;

    }
    return lst;
}

//longLista: Lista x int
template <typename T>
int longLista(const Lista<T> lst)
{
    if(lst == NULL)
        return 0;
    else
    {
        Lista<T> tmp = lst;
        int n = 1;
        while(tmp->sig != NULL)
        {
            tmp = tmp->sig;
            n++;
        }
        return n;
    }
}

//infoLista: Lista x int -> T
template <typename T>
T infoLista(const Lista<T> lst, int pos)
{
    if(pos < 1 || pos > longLista(lst))
        throw std::out_of_range("infoLista: No existe esa posicion");

    Lista<T> tmp = lst;
    for(int i=1; i<pos; i++)
        tmp = tmp->sig;

    return tmp->dato;
}

//insLista: Lista x T x int -> Lista
template <typename T>
Lista<T> insLista(Lista<T> lst, const T e, int pos)
{
    if(pos < 1 || pos > longLista(lst))
        throw std::out_of_range("insLista: No existe esa posicion");

    Lista<T> nuevo = (Lista<T>)malloc(sizeof(Nodo<T>));
    nuevo->dato = e;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if(pos == 1)
    {
        nuevo->sig = lst;
        lst->ant = nuevo;
        lst = nuevo;
    }
    else
    {
        Lista<T> tmp = lst;
        for(int i=1; i<pos-1; i++)
            tmp = tmp->sig;
        nuevo->sig = tmp->sig;
        nuevo->ant = tmp;
        tmp->sig->ant = nuevo;
        tmp->sig = nuevo;
    }

    return lst;
}


template <typename T>
Lista<T> elimLista(Lista<T> lst, int pos)
{
    if(pos < 1 || pos > longLista(lst))
        throw std::out_of_range("elimLista: No existe esa posicion");

    if(pos == 1)
    {
        Lista<T> tmp = lst;
        lst = lst->sig;
        lst->sig = NULL;
        tmp->sig = NULL;
        free(tmp);
    }
    else
    {
        Lista<T> tmp = lst;
        for(int i=1; i<pos-1; i++)
            tmp = tmp->sig;
        Lista<T> tmp2 = tmp->sig;
        tmp->sig = tmp->sig->sig;
        tmp->sig->ant = tmp;
        tmp2->sig = NULL;
        tmp2->ant = NULL;
        free(tmp2);
    }
    return lst;
}

template <typename T>
bool vaciaLista(Lista<T> lst)
{
    if(longLista(lst) != 0)
        return 1;
    else
    return 0;
}


#endif