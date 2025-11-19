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

    /**
     * Precondición: cantidadFerrocarriles >= 1 && cantidadFerrocarriles <= 4
     * Postcondición: Retorna el alquiler según la cantidad de ferrocarriles del dueño
     * Reglas Monopoly:
     *   - 1 ferrocarril: $25
     *   - 2 ferrocarriles: $50
     *   - 3 ferrocarriles: $100
     *   - 4 ferrocarriles: $200
     */
    int obtenerAlquiler(int cantidadFerrocarriles) const {
        if (cantidadFerrocarriles < 1 || cantidadFerrocarriles > 4) {
            return alquiler[0];  // Por seguridad, retornar alquiler base
        }
        return alquiler[cantidadFerrocarriles - 1];  // 1->índice 0, 2->índice 1, etc.
    }

    /**
     * Precondición: Ninguna
     * Postcondición: Muestra información del ferrocarril con alquileres
     */
    void mostrarInfo() const {
        cout << "Ferrocarril: " << nombre << " | Precio: $" << precio
             << " | Dueño: " << (duenio.empty() ? "Sin dueño" : duenio) << endl;
        cout << "  Alquileres: 1 FC=$" << alquiler[0]
             << " | 2 FC=$" << alquiler[1]
             << " | 3 FC=$" << alquiler[2]
             << " | 4 FC=$" << alquiler[3] << endl;
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