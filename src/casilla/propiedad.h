#ifndef PROPIEDAD_H_
#define PROPIEDAD_H_

#include "casilla.h"


class Propiedad : public Casilla {
    int precio;
    int alquiler;
    string duenio;
    string color;
    int alquileres[6];
    int numCasas = 0;
public:
     Propiedad(string n, int p, string c, int rentas[6]) 
        : Casilla(n), precio(p), duenio(""), color(c), numCasas(0) 
    {
        for (int i = 0; i < 6; ++i) {
            alquileres[i] = rentas[i];
        }
    }

    void comprar(string jugador) {
        if (duenio == "") {
            duenio = jugador;
            cout << jugador << " compró " << nombre << " por $" << precio << endl;
        } else {
            cout << "Esta propiedad ya tiene dueño: " << duenio << endl;
        }
    }

    void activar() override {
        if (duenio.empty()) {
            cout << "Propiedad sin dueño: " << nombre << " (Precio $" << precio << ", Color " << color << ")" << endl;
        } else {
            int renta = obtenerAlquiler();
            cout << "Has caído en la propiedad de " << duenio << ": " << nombre
                 << " (" << color << ") - Debes pagar $" << renta << endl;
        }
    }

     void construirCasa() {
        if (duenio.empty()) {
            cout << "No puedes construir en una propiedad sin dueño." << endl;
            return;
        }

        if (numCasas < 5) {
            numCasas++;
            if (numCasas < 5)
                cout << duenio << " construyó una casa en " << nombre << ". Total casas: " << numCasas << endl;
            else
                cout << duenio << " construyó un HOTEL en " << nombre << "!" << endl;
        } else {
            cout << "Ya tienes el máximo (hotel) en esta propiedad." << endl;
        }
    }

    int obtenerAlquiler() const {
        return alquileres[numCasas];
    }

    void mostrarInfo() const {
        cout << "Propiedad: " << nombre << " | Color: " << color
             << " | Precio: $" << precio
             << " | Dueño: " << (duenio.empty() ? "Sin dueño" : duenio)
             << " | Casas: " << (numCasas < 5 ? to_string(numCasas) : "Hotel")
             << " | Alquiler actual: $" << obtenerAlquiler() << endl;
    }

     string getColor() const {
        return color;
    }
};

#endif