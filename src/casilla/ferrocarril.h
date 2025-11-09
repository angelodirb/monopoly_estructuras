#ifndef FERROCARRIL_H_
#define FERROCARRIL_H_

#include "casilla.h"

class Ferrocarril : public Casilla {
    int precio;
    int alquiler[4];
    string duenio;
public:
    Ferrocarril(string n, int p, int rentas[4])
        : Casilla(n), precio(p), duenio("") 
    {
        for (int i = 0; i < 4; i++) alquiler[i] = rentas[i];
    }

    void comprar(string jugador) {
        if (duenio == "") {
            duenio = jugador;
            cout << jugador << " compró el ferrocarril " << nombre << " por $" << precio << endl;
        } else {
            cout << "Este ferrocarril ya tiene dueño: " << duenio << endl;
        }
    }

    void activar() override {
        if (duenio == "") {
            cout << "Ferrocarril sin dueño: " << nombre << " (Precio $" << precio << ")" << endl;
        } else {
            cout << "Has caído en el ferrocarril de " << duenio << ": " << nombre
                 << ". Debes pagar $" << alquiler[0] << endl;
        }
    }
};
#endif
