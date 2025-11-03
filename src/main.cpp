#include "propiedad.h"
#include "listaCircular.h"



int main(){
    Lista<Property> tablero = crearLista<Property>();

    tablero =  anxLista(tablero, Mediterraneo);
    tablero =  anxLista(tablero, Baltico);
    tablero =  anxLista(tablero, Oriental);


    return(0);
}

