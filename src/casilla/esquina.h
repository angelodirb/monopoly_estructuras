#include "casilla.h"

class Esquina : public Casilla {
    string tipo; // "Salida", "Carcel", "Parqueadero", "IrCarcel"
public:
    Esquina(string n, string t) : Casilla(n), tipo(t) {}

    void activar() override {
        if (tipo == "Salida")
            cout << "¡Pasas por la salida! Cobras $200." << endl;
        else if (tipo == "Carcel")
            cout << "Estás de visita en la cárcel." << endl;
        else if (tipo == "Parqueadero")
            cout << "Estás descansando en el parqueadero gratuito." << endl;
        else if (tipo == "IrCarcel")
            cout << "Vas directo a la cárcel. No pases por salida." << endl;
    }
};
