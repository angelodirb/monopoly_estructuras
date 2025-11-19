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
            cout << jugador << " compro el ferrocarril " << nombre << " por $" << precio << endl;
        } else {
            cout << "Este ferrocarril ya tiene dueno: " << duenio << endl;
        }
    }

    void activar() override {
        if (duenio == "") {
            cout << "Ferrocarril sin dueno: " << nombre << " (Precio $" << precio << ")" << endl;
        } else {
            cout << "Has caido en el ferrocarril de " << duenio << ": " << nombre
                 << ". Debes pagar $" << alquiler[0] << endl;
        }
    }

    /**
     * Precondicion: cantidadFerrocarriles >= 1 && cantidadFerrocarriles <= 4
     * Postcondicion: Retorna el alquiler segun la cantidad de ferrocarriles del dueno
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
        return alquiler[cantidadFerrocarriles - 1];  // 1->indice 0, 2->indice 1, etc.
    }

    /**
     * Precondicion: Ninguna
     * Postcondicion: Muestra informacion del ferrocarril con alquileres
     */
    void mostrarInfo() const {
        cout << "Ferrocarril: " << nombre << " | Precio: $" << precio
             << " | Dueno: " << (duenio.empty() ? "Sin dueno" : duenio) << endl;
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