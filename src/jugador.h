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
    bool tieneCartaSalirCarcel;  // TRUE si tiene carta para salir de cárcel gratis
    
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea un jugador con valores por defecto
     */
    Jugador() : nombre(""), dinero(1500), posicion(0), enCarcel(false), 
               turnosCarcel(0), estaQuebrado(false), tieneCartaSalirCarcel(false) {}
    
    /**
     * Precondición: nombreJugador no debe estar vacío
     * Postcondición: Se crea un jugador con el nombre especificado
     */
    Jugador(const std::string& nombreJugador) 
        : nombre(nombreJugador), dinero(1500), posicion(0), enCarcel(false),
          turnosCarcel(0), estaQuebrado(false), tieneCartaSalirCarcel(false) {}
};

// **********************************
// Funciones del TAD Jugador
// **********************************

/**
 * Precondición: nombre no debe estar vacío
 * Postcondición: Se crea un nuevo jugador con dinero inicial de $1500
 */
Jugador crearJugador(const std::string& nombre) {
    Jugador j(nombre);
    std::cout << "👤 Jugador creado: " << nombre << " con $" << j.dinero << std::endl;
    return j;
}

/**
 * Precondición: jugador válido, casillas >= 0
 * Postcondición: Mueve el jugador el número de casillas especificado (circular)
 */
void moverJugador(Jugador& jugador, int casillas) {
    int posicionAnterior = jugador.posicion;
    jugador.posicion = (jugador.posicion + casillas) % 40; // Tablero de 40 casillas
    
    std::cout << "🚶 " << jugador.nombre << " se mueve de casilla " 
              << posicionAnterior << " a " << jugador.posicion 
              << " (+" << casillas << ")" << std::endl;
    
    // Verificar si pasó por SALIDA (casilla 0)
    if (posicionAnterior > jugador.posicion || 
        (posicionAnterior + casillas) >= 40) {
        std::cout << "🎯 " << jugador.nombre << " pasó por SALIDA!" << std::endl;
    }
}

/**
 * Precondición: jugador válido, cantidad > 0
 * Postcondición: Agrega dinero al jugador
 */
void agregarDinero(Jugador& jugador, int cantidad) {
    if (cantidad <= 0) {
        std::cout << "❌ No se puede agregar cantidad negativa o cero" << std::endl;
        return;
    }
    
    jugador.dinero += cantidad;
    std::cout << "💰 " << jugador.nombre << " recibe $" << cantidad 
              << ". Nuevo saldo: $" << jugador.dinero << std::endl;
}

/**
 * Precondición: jugador válido, cantidad > 0
 * Postcondición: Retira dinero del jugador, retorna true si exitoso
 */
bool retirarDinero(Jugador& jugador, int cantidad) {
    if (cantidad <= 0) {
        std::cout << "❌ No se puede retirar cantidad negativa o cero" << std::endl;
        return false;
    }
    
    if (jugador.dinero < cantidad) {
        std::cout << "💸 " << jugador.nombre << " no tiene fondos suficientes. " 
                  << "Saldo: $" << jugador.dinero << ", Necesita: $" << cantidad << std::endl;
        return false;
    }
    
    jugador.dinero -= cantidad;
    std::cout << "💸 " << jugador.nombre << " paga $" << cantidad 
              << ". Nuevo saldo: $" << jugador.dinero << std::endl;
    
    return true;
}

/**
 * Precondición: jugadores válidos, cantidad > 0
 * Postcondición: Transfiere dinero entre jugadores, retorna true si exitoso
 */
bool pagarA(Jugador& pagador, Jugador& receptor, int cantidad) {
    if (cantidad <= 0) {
        std::cout << "❌ No se puede transferir cantidad negativa o cero" << std::endl;
        return false;
    }
    
    if (retirarDinero(pagador, cantidad)) {
        agregarDinero(receptor, cantidad);
        std::cout << "🔄 Transferencia: " << pagador.nombre << " → " << receptor.nombre 
                  << " $" << cantidad << std::endl;
        return true;
    }
    
    return false;
}

/**
 * Precondición: jugador válido, nombre de propiedad no vacío, precio > 0
 * Postcondición: Compra una propiedad si tiene fondos suficientes
 */
bool comprarPropiedad(Jugador& jugador, const std::string& nombrePropiedad, int precio) {
    if (nombrePropiedad.empty() || precio <= 0) {
        std::cout << "❌ Datos de propiedad inválidos" << std::endl;
        return false;
    }
    
    if (retirarDinero(jugador, precio)) {
        jugador.propiedades.push_back(nombrePropiedad);
        std::cout << "🏠 " << jugador.nombre << " compró " << nombrePropiedad 
                  << " por $" << precio << std::endl;
        std::cout << "📋 Propiedades actuales: " << jugador.propiedades.size() << std::endl;
        return true;
    }
    
    return false;
}

/**
 * Precondición: jugador válido
 * Postcondición: Envía al jugador a la cárcel
 */
void enviarACarcel(Jugador& jugador) {
    jugador.posicion = 10; // Casilla de la cárcel
    jugador.enCarcel = true;
    jugador.turnosCarcel = 0;
    
    std::cout << "🔒 " << jugador.nombre << " va a la CÁRCEL!" << std::endl;
}

/**
 * Precondición: jugador válido y en la cárcel
 * Postcondición: Libera al jugador de la cárcel
 */
void liberarDeCarcel(Jugador& jugador) {
    if (!jugador.enCarcel) {
        std::cout << "⚠️ " << jugador.nombre << " no está en la cárcel" << std::endl;
        return;
    }
    
    jugador.enCarcel = false;
    jugador.turnosCarcel = 0;
    std::cout << "🔓 " << jugador.nombre << " sale de la CÁRCEL!" << std::endl;
}

/**
 * Precondición: jugador válido
 * Postcondición: El jugador obtiene una carta de "Salir de Cárcel"
 */
void darCartaSalirCarcel(Jugador& jugador) {
    jugador.tieneCartaSalirCarcel = true;
    std::cout << "🎴 " << jugador.nombre << " obtuvo carta 'Salir de Cárcel Gratis'!" << std::endl;
}

/**
 * Precondición: jugador válido y debe tener carta de salir de cárcel
 * Postcondición: Usa la carta para salir de cárcel, la carta se consume (FALSE)
 */
bool usarCartaSalirCarcel(Jugador& jugador) {
    if (!jugador.tieneCartaSalirCarcel) {
        std::cout << "❌ " << jugador.nombre << " no tiene carta para salir de cárcel" << std::endl;
        return false;
    }
    
    if (!jugador.enCarcel) {
        std::cout << "⚠️ " << jugador.nombre << " no está en la cárcel" << std::endl;
        return false;
    }
    
    // Usar la carta: sale de cárcel gratis
    liberarDeCarcel(jugador);
    jugador.tieneCartaSalirCarcel = false;  // Se consume la carta
    
    std::cout << "🎴 " << jugador.nombre << " usó su carta 'Salir de Cárcel' y queda libre!" << std::endl;
    return true;
}

/**
 * Precondición: jugador válido y en cárcel
 * Postcondición: Intenta salir de cárcel por diferentes métodos
 */
bool intentarSalirDeCarcel(Jugador& jugador) {
    if (!jugador.enCarcel) {
        std::cout << "⚠️ " << jugador.nombre << " no está en la cárcel" << std::endl;
        return false;
    }
    
    std::cout << "\n🔒 " << jugador.nombre << " está en la CÁRCEL" << std::endl;
    std::cout << "Turnos en cárcel: " << jugador.turnosCarcel << "/3" << std::endl;
    
    // Verificar si tiene carta para salir gratis
    if (jugador.tieneCartaSalirCarcel) {
        std::cout << "🎴 ¡Tienes carta 'Salir de Cárcel'!" << std::endl;
        std::cout << "¿Usar carta para salir gratis? (s/n): ";
        
        // En implementación real, aquí sería input del usuario
        // Por ahora usamos automáticamente si está disponible
        return usarCartaSalirCarcel(jugador);
    }
    
    // Si no tiene carta, debe pagar multa o esperar 3 turnos
    jugador.turnosCarcel++;
    
    if (jugador.turnosCarcel >= 3) {
        std::cout << "⏰ 3 turnos completados - sale automáticamente" << std::endl;
        liberarDeCarcel(jugador);
        return true;
    }
    
    std::cout << "💸 Opciones: Pagar $50 de multa o esperar " 
              << (3 - jugador.turnosCarcel) << " turno(s) más" << std::endl;
    
    return false; // Sigue en cárcel
}

/**
 * Precondición: jugador válido
 * Postcondición: Muestra el estado completo del jugador
 */
void mostrarEstadoJugador(const Jugador& jugador) {
    std::cout << "\n=== ESTADO DE " << jugador.nombre << " ===" << std::endl;
    std::cout << "💰 Dinero: $" << jugador.dinero << std::endl;
    std::cout << "📍 Posición: " << jugador.posicion << std::endl;
    std::cout << "🏠 Propiedades: " << jugador.propiedades.size() << std::endl;
    
    for (size_t i = 0; i < jugador.propiedades.size(); i++) {
        std::cout << "  " << (i+1) << ". " << jugador.propiedades[i] << std::endl;
    }
    
    if (jugador.enCarcel) {
        std::cout << "🔒 Estado: EN CÁRCEL (turno " << jugador.turnosCarcel << ")" << std::endl;
    }
    
    if (jugador.estaQuebrado) {
        std::cout << "💀 Estado: QUEBRADO" << std::endl;
    }
    
    if (jugador.tieneCartaSalirCarcel) {
        std::cout << "🎴 Carta 'Salir de Cárcel': ✅ DISPONIBLE" << std::endl;
    } else {
        std::cout << "🎴 Carta 'Salir de Cárcel': ❌ No disponible" << std::endl;
    }
    
    std::cout << "================================" << std::endl;
}

/**
 * Precondición: jugador válido
 * Postcondición: Retorna true si el jugador está en quiebra
 */
bool estaEnQuiebra(const Jugador& jugador) {
    return jugador.estaQuebrado || jugador.dinero <= 0;
}

/**
 * Precondición: jugador válido
 * Postcondición: Retorna true si el jugador puede salir de cárcel de alguna manera
 */
bool puedeUsarCartaSalirCarcel(const Jugador& jugador) {
    return jugador.tieneCartaSalirCarcel && jugador.enCarcel;
}

/**
 * Precondición: jugador válido
 * Postcondición: Retorna true si el jugador posee la propiedad
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