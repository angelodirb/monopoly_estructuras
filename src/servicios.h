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

    /**
     * Precondición: sumaDados > 0, cantidadServicios >= 1 && cantidadServicios <= 2
     * Postcondición: Retorna el alquiler según los dados y cantidad de servicios
     * Reglas Monopoly:
     *   - 1 servicio: 4 × suma de dados
     *   - 2 servicios (ambos): 10 × suma de dados
     */
    int obtenerAlquiler(int sumaDados, int cantidadServicios) const {
        if (cantidadServicios == 2) {
            return sumaDados * 10;  // Tiene ambos servicios
        } else {
            return sumaDados * 4;   // Solo tiene un servicio
        }
    }

    /**
     * Precondición: Ninguna
     * Postcondición: Muestra información del servicio
     */
    void mostrarInfo() const {
        cout << "Servicio: " << nombre << " | Precio: $" << precio
             << " | Dueño: " << (duenio.empty() ? "Sin dueño" : duenio) << endl;
        cout << "  Alquiler: 4× dados (1 servicio) | 10× dados (2 servicios)" << endl;
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