#include "casilla.h"

class Ferrocarril : public Casilla {
    int precio;
    int alquiler;
    string duenio;
public:
    Ferrocarril(string n, int p, int a)
        : Casilla(n), precio(p), alquiler(a), duenio("") {}

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
            cout << "Has caído en el ferrocarril de " << duenio
                 << ": " << nombre << " (Debes pagar $" << alquiler << ")" << endl;
        }
    }
};
