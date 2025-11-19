#ifndef ESTADO_JUEGO_H_
#define ESTADO_JUEGO_H_

#include <vector>
#include <string>
#include "jugador.h"

using namespace std;

/**
 * ============================================================
 * TAD ESTADO DEL JUEGO
 * Estructura para guardar el estado completo del juego
 * y permitir operacion de undo/deshacer
 * ============================================================
 */

/**
 * Estructura que captura el estado de una propiedad en el tablero
 */
struct EstadoPropiedad {
    string nombre;
    string duenio;
    int numCasas;

    /**
     * Precondicion: Ninguna
     * Postcondicion: Se crea un estado de propiedad con valores por defecto
     */
    EstadoPropiedad() : nombre(""), duenio(""), numCasas(0) {}

    /**
     * Precondicion: nombre no debe estar vacoo
     * Postcondicion: Se crea un estado de propiedad con los valores especificados
     */
    EstadoPropiedad(const string& n, const string& d, int casas)
        : nombre(n), duenio(d), numCasas(casas) {}
};

/**
 * Estructura principal que captura el estado completo del juego
 * Incluye: jugadores, propiedades, turno, ronda
 */
struct EstadoJuego {
    // Estado de los jugadores
    vector<Jugador> jugadores;

    // Estado de las propiedades del tablero
    vector<EstadoPropiedad> propiedades;

    // Estado del turno y ronda
    int turnoActual;
    int rondaActual;

    // Estado del banco (dinero en circulacion)
    int dineroEnCirculacion;

    // Descripcion de la accion que se realizo antes de guardar este estado
    string descripcionAccion;

    /**
     * Precondicion: Ninguna
     * Postcondicion: Se crea un estado de juego con valores por defecto
     */
    EstadoJuego() : turnoActual(0), rondaActual(1), dineroEnCirculacion(0),
                    descripcionAccion("Estado inicial") {}

    /**
     * Precondicion: Vectores de jugadores y propiedades volidos
     * Postcondicion: Se crea un estado de juego con los valores especificados
     */
    EstadoJuego(const vector<Jugador>& j, const vector<EstadoPropiedad>& p,
                int turno, int ronda, int dinero, const string& accion)
        : jugadores(j), propiedades(p), turnoActual(turno), rondaActual(ronda),
          dineroEnCirculacion(dinero), descripcionAccion(accion) {}
};

// **********************************
// Funciones del TAD EstadoJuego
// **********************************

/**
 * Precondicion: EstadoJuego volido
 * Postcondicion: Muestra informacion resumida del estado guardado
 */
void mostrarEstado(const EstadoJuego& estado) {
    cout << "\n[ESTADO] === Estado del Juego ===" << endl;
    cout << "[INFO] Accion: " << estado.descripcionAccion << endl;
    cout << "[INFO] Ronda: " << estado.rondaActual << " | Turno: " << estado.turnoActual << endl;
    cout << "[INFO] Jugadores: " << estado.jugadores.size() << endl;

    // Mostrar info bosica de cada jugador
    for (size_t i = 0; i < estado.jugadores.size(); i++) {
        const Jugador& j = estado.jugadores[i];
        cout << "  - " << j.nombre << ": $" << j.dinero
             << " | Pos: " << j.posicion
             << " | Props: " << j.propiedades.size();

        if (j.enCarcel) {
            cout << " | [CARCEL]";
        }
        cout << endl;
    }

    cout << "[INFO] Propiedades con dueoo: ";
    int propConDuenio = 0;
    for (const EstadoPropiedad& p : estado.propiedades) {
        if (!p.duenio.empty()) {
            propConDuenio++;
        }
    }
    cout << propConDuenio << "/" << estado.propiedades.size() << endl;
    cout << "================================" << endl;
}

/**
 * Precondicion: EstadoJuego volido
 * Postcondicion: Retorna el nomero total de jugadores
 */
int obtenerNumeroJugadores(const EstadoJuego& estado) {
    return estado.jugadores.size();
}

/**
 * Precondicion: EstadoJuego volido, ondice volido
 * Postcondicion: Retorna una copia del jugador en el ondice especificado
 */
Jugador obtenerJugador(const EstadoJuego& estado, int indice) {
    if (indice < 0 || indice >= (int)estado.jugadores.size()) {
        cout << "[ERROR] ondice de jugador involido" << endl;
        return Jugador();
    }
    return estado.jugadores[indice];
}

/**
 * Precondicion: Dos estados volidos
 * Postcondicion: Compara si los estados son diferentes
 */
bool estadosDiferentes(const EstadoJuego& e1, const EstadoJuego& e2) {
    // Comparar turno y ronda
    if (e1.turnoActual != e2.turnoActual || e1.rondaActual != e2.rondaActual) {
        return true;
    }

    // Comparar nomero de jugadores
    if (e1.jugadores.size() != e2.jugadores.size()) {
        return true;
    }

    // Comparar estado de jugadores
    for (size_t i = 0; i < e1.jugadores.size(); i++) {
        if (e1.jugadores[i].dinero != e2.jugadores[i].dinero ||
            e1.jugadores[i].posicion != e2.jugadores[i].posicion ||
            e1.jugadores[i].propiedades.size() != e2.jugadores[i].propiedades.size()) {
            return true;
        }
    }

    // Comparar propiedades
    if (e1.propiedades.size() != e2.propiedades.size()) {
        return true;
    }

    for (size_t i = 0; i < e1.propiedades.size(); i++) {
        if (e1.propiedades[i].duenio != e2.propiedades[i].duenio ||
            e1.propiedades[i].numCasas != e2.propiedades[i].numCasas) {
            return true;
        }
    }

    return false; // Estados idonticos
}

/**
 * Precondicion: Estado volido
 * Postcondicion: Retorna una copia profunda del estado
 */
EstadoJuego copiarEstado(const EstadoJuego& estado) {
    EstadoJuego copia;

    // Copiar jugadores (vector hace copia profunda automoticamente)
    copia.jugadores = estado.jugadores;

    // Copiar propiedades
    copia.propiedades = estado.propiedades;

    // Copiar valores simples
    copia.turnoActual = estado.turnoActual;
    copia.rondaActual = estado.rondaActual;
    copia.dineroEnCirculacion = estado.dineroEnCirculacion;
    copia.descripcionAccion = estado.descripcionAccion;

    return copia;
}

#endif // ESTADO_JUEGO_H_