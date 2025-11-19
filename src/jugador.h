#ifndef __JUGADOR_H
#define __JUGADOR_H

#include <string>
#include <vector>
#include <iostream>

// **********************************
// Estructura del Jugador
// **********************************

struct Jugador {
    std::string nombre;
    int dinero;
    int posicion;
    std::vector<std::string> propiedades;
    bool enCarcel;
    int turnosCarcel;
    bool estaQuebrado;
    bool tieneCartaSalirCarcel;  // TRUE si tiene carta para salir de carcel gratis
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Se crea un jugador con valores por defecto
     */
    Jugador() : nombre(""), dinero(1500), posicion(0), enCarcel(false), 
               turnosCarcel(0), estaQuebrado(false), tieneCartaSalirCarcel(false) {}
    
    /**
     * Precondicion: nombreJugador no debe estar vacio
     * Postcondicion: Se crea un jugador con el nombre especificado
     */
    Jugador(const std::string& nombreJugador) 
        : nombre(nombreJugador), dinero(1500), posicion(0), enCarcel(false),
          turnosCarcel(0), estaQuebrado(false), tieneCartaSalirCarcel(false) {}
};

// **********************************
// Funciones del TAD Jugador
// **********************************

/**
 * Precondicion: nombre no debe estar vacio
 * Postcondicion: Se crea un nuevo jugador con dinero inicial de $1500
 */
Jugador crearJugador(const std::string& nombre) {
    Jugador j(nombre);
    std::cout << "[JUGADOR] Jugador creado: " << nombre << " con $" << j.dinero << std::endl;
    return j;
}

/**
 * Precondicion: jugador valido, casillas >= 0
 * Postcondicion: Mueve el jugador el numero de casillas especificado (circular)
 */
void moverJugador(Jugador& jugador, int casillas) {
    int posicionAnterior = jugador.posicion;
    jugador.posicion = (jugador.posicion + casillas) % 40; // Tablero de 40 casillas
    
    std::cout << "🚶 " << jugador.nombre << " se mueve de casilla " 
              << posicionAnterior << " a " << jugador.posicion 
              << " (+" << casillas << ")" << std::endl;
    
    // Verificar si paso por SALIDA (casilla 0)
    if (posicionAnterior > jugador.posicion || 
        (posicionAnterior + casillas) >= 40) {
        std::cout << ">>> " << jugador.nombre << " paso por SALIDA!" << std::endl;
    }
}

/**
 * Precondicion: jugador valido, cantidad > 0
 * Postcondicion: Agrega dinero al jugador
 */
void agregarDinero(Jugador& jugador, int cantidad) {
    if (cantidad <= 0) {
        std::cout << "[ERROR] No se puede agregar cantidad negativa o cero" << std::endl;
        return;
    }
    
    jugador.dinero += cantidad;
    std::cout << "[$] " << jugador.nombre << " recibe $" << cantidad 
              << ". Nuevo saldo: $" << jugador.dinero << std::endl;
}

/**
 * Precondicion: jugador valido, cantidad > 0
 * Postcondicion: Retira dinero del jugador, retorna true si exitoso
 */
bool retirarDinero(Jugador& jugador, int cantidad) {
    if (cantidad <= 0) {
        std::cout << "[ERROR] No se puede retirar cantidad negativa o cero" << std::endl;
        return false;
    }
    
    if (jugador.dinero < cantidad) {
        std::cout << jugador.nombre << " no tiene fondos suficientes. " 
                  << "Saldo: $" << jugador.dinero << ", Necesita: $" << cantidad << std::endl;
        return false;
    }
    
    jugador.dinero -= cantidad;
    std::cout << jugador.nombre << " paga $" << cantidad 
              << ". Nuevo saldo: $" << jugador.dinero << std::endl;
    
    return true;
}

/**
 * Precondicion: jugadores validos, cantidad > 0
 * Postcondicion: Transfiere dinero entre jugadores, retorna true si exitoso
 */
bool pagarA(Jugador& pagador, Jugador& receptor, int cantidad) {
    if (cantidad <= 0) {
        std::cout << "[ERROR] No se puede transferir cantidad negativa o cero" << std::endl;
        return false;
    }
    
    if (retirarDinero(pagador, cantidad)) {
        agregarDinero(receptor, cantidad);
        std::cout << "[MEZCLAR] Transferencia: " << pagador.nombre << " → " << receptor.nombre 
                  << " $" << cantidad << std::endl;
        return true;
    }
    
    return false;
}

/**
 * Precondicion: jugador valido, nombre de propiedad no vacio, precio > 0
 * Postcondicion: Compra una propiedad si tiene fondos suficientes
 */
bool comprarPropiedad(Jugador& jugador, const std::string& nombrePropiedad, int precio) {
    if (nombrePropiedad.empty() || precio <= 0) {
        std::cout << "[ERROR] Datos de propiedad invalidos" << std::endl;
        return false;
    }
    
    if (retirarDinero(jugador, precio)) {
        jugador.propiedades.push_back(nombrePropiedad);
        std::cout << "[PROP] " << jugador.nombre << " compro " << nombrePropiedad 
                  << " por $" << precio << std::endl;
        std::cout << "[INFO] Propiedades actuales: " << jugador.propiedades.size() << std::endl;
        return true;
    }
    
    return false;
}

/**
 * Precondicion: jugador valido
 * Postcondicion: Envia al jugador a la carcel
 */
void enviarACarcel(Jugador& jugador) {
    jugador.posicion = 10; // Casilla de la carcel
    jugador.enCarcel = true;
    jugador.turnosCarcel = 0;
    
    std::cout << jugador.nombre << " va a la CARCEL!" << std::endl;
}

/**
 * Precondicion: jugador valido y en la carcel
 * Postcondicion: Libera al jugador de la carcel
 */
void liberarDeCarcel(Jugador& jugador) {
    if (!jugador.enCarcel) {
        std::cout << "[AVISO] " << jugador.nombre << " no esta en la carcel" << std::endl;
        return;
    }
    
    jugador.enCarcel = false;
    jugador.turnosCarcel = 0;
    std::cout << jugador.nombre << " sale de la CARCEL!" << std::endl;
}

/**
 * Precondicion: jugador valido
 * Postcondicion: El jugador obtiene una carta de "Salir de Carcel"
 */
void darCartaSalirCarcel(Jugador& jugador) {
    jugador.tieneCartaSalirCarcel = true;
    std::cout << "[CARTA] " << jugador.nombre << " obtuvo carta 'Salir de Carcel Gratis'!" << std::endl;
}

/**
 * Precondicion: jugador valido y debe tener carta de salir de carcel
 * Postcondicion: Usa la carta para salir de carcel, la carta se consume (FALSE)
 */
bool usarCartaSalirCarcel(Jugador& jugador) {
    if (!jugador.tieneCartaSalirCarcel) {
        std::cout << "[ERROR] " << jugador.nombre << " no tiene carta para salir de carcel" << std::endl;
        return false;
    }
    
    if (!jugador.enCarcel) {
        std::cout << "[AVISO] " << jugador.nombre << " no esta en la carcel" << std::endl;
        return false;
    }
    
    // Usar la carta: sale de carcel gratis
    liberarDeCarcel(jugador);
    jugador.tieneCartaSalirCarcel = false;  // Se consume la carta
    
    std::cout << "[CARTA] " << jugador.nombre << " uso su carta 'Salir de Carcel' y queda libre!" << std::endl;
    return true;
}

/**
 * Precondicion: jugador valido y en carcel
 * Postcondicion: Intenta salir de carcel por diferentes metodos
 */
bool intentarSalirDeCarcel(Jugador& jugador) {
    if (!jugador.enCarcel) {
        std::cout << "[AVISO] " << jugador.nombre << " no esta en la carcel" << std::endl;
        return false;
    }
    
    std::cout << "\n " << jugador.nombre << " esta en la CARCEL" << std::endl;
    std::cout << "Turnos en carcel: " << jugador.turnosCarcel << "/3" << std::endl;
    
    // Verificar si tiene carta para salir gratis
    if (jugador.tieneCartaSalirCarcel) {
        std::cout << "[CARTA] [!]Tienes carta 'Salir de Carcel'!" << std::endl;
        std::cout << "[ ?]Usar carta para salir gratis? (s/n): ";
        
        // En implementacion real, aqui seria input del usuario
        // Por ahora usamos automaticamente si esta disponible
        return usarCartaSalirCarcel(jugador);
    }
    
    // Si no tiene carta, debe pagar multa o esperar 3 turnos
    jugador.turnosCarcel++;
    
    if (jugador.turnosCarcel >= 3) {
        std::cout << " 3 turnos completados - sale automaticamente" << std::endl;
        liberarDeCarcel(jugador);
        return true;
    }
    
    std::cout << " Opciones: Pagar $50 de multa o esperar " 
              << (3 - jugador.turnosCarcel) << " turno(s) mas" << std::endl;
    
    return false; // Sigue en carcel
}

/**
 * Precondicion: jugador valido
 * Postcondicion: Muestra el estado completo del jugador
 */
void mostrarEstadoJugador(const Jugador& jugador) {
    std::cout << "\n=== ESTADO DE " << jugador.nombre << " ===" << std::endl;
    std::cout << "[$] Dinero: $" << jugador.dinero << std::endl;
    std::cout << " Posicion: " << jugador.posicion << std::endl;
    std::cout << "[PROP] Propiedades: " << jugador.propiedades.size() << std::endl;
    
    for (size_t i = 0; i < jugador.propiedades.size(); i++) {
        std::cout << "  " << (i+1) << ". " << jugador.propiedades[i] << std::endl;
    }
    
    if (jugador.enCarcel) {
        std::cout << "Estado: EN CARCEL (turno " << jugador.turnosCarcel << ")" << std::endl;
    }
    
    if (jugador.estaQuebrado) {
        std::cout << "Estado: QUEBRADO" << std::endl;
    }
    
    if (jugador.tieneCartaSalirCarcel) {
        std::cout << "[CARTA] Carta 'Salir de Carcel': [OK] DISPONIBLE" << std::endl;
    } else {
        std::cout << "[CARTA] Carta 'Salir de Carcel': [ERROR] No disponible" << std::endl;
    }
    
    std::cout << "================================" << std::endl;
}

/**
 * Precondicion: jugador valido
 * Postcondicion: Retorna true si el jugador esta en quiebra
 */
bool estaEnQuiebra(const Jugador& jugador) {
    return jugador.estaQuebrado || jugador.dinero <= 0;
}

/**
 * Precondicion: jugador valido
 * Postcondicion: Retorna true si el jugador puede salir de carcel de alguna manera
 */
bool puedeUsarCartaSalirCarcel(const Jugador& jugador) {
    return jugador.tieneCartaSalirCarcel && jugador.enCarcel;
}

/**
 * Precondicion: jugador valido
 * Postcondicion: Retorna true si el jugador posee la propiedad
 */
bool poseePropiedad(const Jugador& jugador, const std::string& nombrePropiedad) {
    for (const std::string& prop : jugador.propiedades) {
        if (prop == nombrePropiedad) {
            return true;
        }
    }
    return false;
}

#endif // __JUGADOR_H