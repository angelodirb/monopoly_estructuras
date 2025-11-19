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
            cout << jugador << " compro el servicio " << nombre << " por $" << precio << endl;
        } else {
            cout << "Este servicio ya tiene dueño: " << duenio << endl;
        }
    }

    void activar() override {
        if (duenio == "") {
            cout << "Servicio sin dueno: " << nombre << " (Precio $" << precio << ")" << endl;
        } else {
            cout << "Has caido en el servicio de " << duenio << ": " << nombre << endl;
            cout << "Debes pagar segun la tirada del dado." << endl;
        }
    }

    /**
     * Precondicion: sumaDados > 0, cantidadServicios >= 1 && cantidadServicios <= 2
     * Postcondicion: Retorna el alquiler segun los dados y cantidad de servicios
     * Reglas Monopoly:
     *   - 1 servicio: 4 x suma de dados
     *   - 2 servicios (ambos): 10 x suma de dados
     */
    int obtenerAlquiler(int sumaDados, int cantidadServicios) const {
        if (cantidadServicios == 2) {
            return sumaDados * 10;  // Tiene ambos servicios
        } else {
            return sumaDados * 4;   // Solo tiene un servicio
        }
    }

    /**
     * Precondicion: Ninguna
     * Postcondicion: Muestra informacion del servicio
     */
    void mostrarInfo() const {
        cout << "Servicio: " << nombre << " | Precio: $" << precio
             << " | Dueño: " << (duenio.empty() ? "Sin dueño" : duenio) << endl;
        cout << "  Alquiler: 4x dados (1 servicio) | 10x dados (2 servicios)" << endl;
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