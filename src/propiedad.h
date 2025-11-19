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

    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el precio base de la propiedad
     */
    int getPrecio() const {
        return precio;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el dueño de la propiedad
     */
    string getDuenio() const {
        return duenio;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el número de casas construidas
     */
    int getNumCasas() const {
        return numCasas;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el valor de las mejoras (casas/hoteles)
     */
    int getValorMejoras() const {
        // Aproximación: cada casa vale 50% del costo de construcción
        // Hotel vale el doble
        if (numCasas < 5) {
            return numCasas * 50;  // $50 por casa
        } else {
            return 500;  // Hotel completo
        }
    }
    
    /**
     * Precondición: nombre de jugador válido
     * Postcondición: Establece el dueño de la propiedad
     */
    void setDuenio(const string& nuevoDuenio) {
        duenio = nuevoDuenio;
    }

    /**
     * Precondición: numCasas >= 0 && numCasas <= 5
     * Postcondición: Establece el número de casas/hotel en la propiedad
     */
    void setNumCasas(int nuevasCasas) {
        if (nuevasCasas >= 0 && nuevasCasas <= 5) {
            numCasas = nuevasCasas;
        }
    }
};

#endif