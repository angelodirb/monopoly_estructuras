#ifndef JUEGO_H_
#define JUEGO_H_

#include "tablero.h"
#include "Banco.h"           // ← CAMBIO: Incluir Banco externo
#include "cola.h"
#include "EstadoJuego.h"
#include "CartaJugador/jugador.h"
#include "CartaJugador/cartas.h"
#include <vector>
#include <iostream>

class Juego {
private:
    ListaCircular tablero;
    Banco banco;             // ← CAMBIO: Usar struct Banco
    Cola<Carta> colaSuerte;
    Cola<Carta> colaCofre;
    
    std::vector<Jugador> jugadores;
    int turnoActual;
    int rondaActual;
    bool juegoActivo;
    
public:
    Juego() : turnoActual(0), rondaActual(1), juegoActivo(false) {
        // Inicializar banco
        banco = Banco();
        colaSuerte = crearCola<Carta>();
        colaCofre = crearCola<Carta>();
        srand(time(0));
    }
    
    bool inicializarJuego() {
        // ... cargar tablero, etc.
        
        // ← CAMBIO: Usar función externa
        bancoDistribuirDineroInicial(banco, jugadores, 1500);
        
        // ...
    }
    
    // Resto igual, pero usando bancoXXX() en lugar de this->bancoXXX()
};

#endif