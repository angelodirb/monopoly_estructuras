#ifndef SERVICIOS_H_
#define SERVICIOS_H_
#include "casilla.h"

class Servicio : public Casilla {
    int precio;
    string duenio;
public:
    Servicio(string n, int p) : Casilla(n), precio(p), duenio("") {}

    void comprar(string jugador) {
        if (duenio == "") {
            duenio = jugador;
            cout << jugador << " compró el servicio " << nombre << " por $" << precio << endl;
        } else {
            cout << "Este servicio ya tiene dueño: " << duenio << endl;
        }
    }

    void activar() override {
        if (duenio == "") {
            cout << "Servicio sin dueño: " << nombre << " (Precio $" << precio << ")" << endl;
        } else {
            cout << "Has caído en el servicio de " << duenio << ": " << nombre << endl;
            cout << "Debes pagar según la tirada del dado." << endl;
        }
    }
    int getPrecio() const {
        return precio;
    }
    
    string getDuenio() const {
        return duenio;
    }
    
    void setDuenio(const string& nuevoDuenio) {
        duenio = nuevoDuenio;
    }
};

#endif
