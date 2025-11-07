#include "casilla.h"


class Propiedad : public Casilla {
    int precio;
    int alquiler;
    string duenio;
public:
    Propiedad(string n, int p, int a)
        : Casilla(n), precio(p), alquiler(a), duenio("") {}

    void comprar(string jugador) {
        if (duenio == "") {
            duenio = jugador;
            cout << jugador << " compró " << nombre << " por $" << precio << endl;
        } else {
            cout << "Esta propiedad ya tiene dueño: " << duenio << endl;
        }
    }

    void activar() override {
        if (duenio == "") {
            cout << "Propiedad sin dueño: " << nombre << " (Precio $" << precio << ")" << endl;
        } else {
            cout << "Has caído en la propiedad de " << duenio << ": " << nombre
                 << " (Debes pagar $" << alquiler << ")" << endl;
        }
    }
};
