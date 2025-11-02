#ifndef PROPIEDAD_H_
#define PROPIEDAD_H_

#include <vector>
#include <string>
using namespace std;


struct Property{
    int price;
    string name;
    int nivel = 0;
    string propietary;
    int rent;
    bool occupied;
    string color;
};

// GRUPO CAFE
Property Mediterráneo = {60, "Mediterraneo", 0, "NN", 2, false, "CAFE"};
Property Báltico = {60, "Baltico", 0, "NN", 4, false, "CAFE"};

// GRUPO AZUL CLARO
Property Oriental = {100, "Oriental", 0, "NN", 6, false, "AZUL_CLARO"};
Property Vermont = {100, "Vermont", 0, "NN", 6, false, "AZUL_CLARO"};
Property Connecticut = {120, "Connecticut", 0, "NN", 8, false, "AZUL_CLARO"};

// GRUPO ROSA 
Property San_Carlos = {140, "San Carlos", 0, "NN", 10, false, "ROSA"};
Property Estados = {140, "Estados", 0, "NN", 10, false, "ROSA"};
Property Virginia = {160, "Virginia", 0, "NN", 12, false, "ROSA"};

// GRUPO NARANJA
Property St_James_Place = {180, "St James Place", 0, "NN", 14, false, "NARANJA"};
Property Tennessee = {180, "Tennessee", 0, "NN", 14, false, "NARANJA"};
Property Nueva_York = {200, "Nueva York", 0, "NN", 16, false, "NARANJA"};

// GRUPO ROJO
Property Kentucky = {220, "Kentucky", 0, "NN", 18, false, "ROJO"};
Property Indiana = {220, "Indiana", 0, "NN", 18, false, "ROJO"};
Property Illinois = {240, "Illinois", 0, "NN", 20, false, "ROJO"};

// GRUPO AMARILLO
Property Atlantic = {260, "Atlantic", 0, "NN", 22, false, "AMARILLO"};
Property Ventnor = {260, "Ventnor", 0, "NN", 22, false, "AMARILLO"};
Property Marvin_Gardens = {280, "Marvin Gardens", 0, "NN", 24, false, "AMARILLO"};

// GRUPO VERDE
Property Pacífico = {300, "Pacifico", 0, "NN", 26, false, "VERDE"};
Property Carolina_del_Norte = {300, "Carolina del Norte", 0, "NN", 26, false, "VERDE"};
Property Pensilvania = {320, "Pensilvania", 0, "NN", 28, false, "VERDE"};

// GRUPO AZUL OSCURO
Property Park_Place = {350, "Park Place", 0, "NN", 35, false, "AZUL_OSCURO"};
Property Boardwalk = {400, "Boardwalk", 0, "NN", 50, false, "AZUL_OSCURO"};


#endif



