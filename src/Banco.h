#ifndef BANCO_H_
#define BANCO_H_

#include "CartaJugador/jugador.h"
#include <string>
#include <vector>
#include <iostream>

// **********************************
// Estructura para historial de transacciones
// **********************************
struct Transaccion {
    std::string tipo;           // "PAGO", "COBRO", "TRANSFERENCIA", etc.
    std::string deJugador;
    std::string aJugador;
    int monto;
    std::string concepto;
    int turno;
    
    Transaccion() : tipo(""), deJugador(""), aJugador(""), monto(0), 
                    concepto(""), turno(0) {}
    
    Transaccion(const std::string& t, const std::string& de, 
                const std::string& a, int m, const std::string& c, int tu)
        : tipo(t), deJugador(de), aJugador(a), monto(m), concepto(c), turno(tu) {}
};

// **********************************
// TAD BANCO
// **********************************
struct Banco {
    int dineroEnCirculacion;
    int dineroRetenido;          // Propiedades hipotecadas
    std::vector<Transaccion> historial;
    
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea un banco con dinero inicial
     */
    Banco() : dineroEnCirculacion(0), dineroRetenido(0) {}
};

// **********************************
// OPERACIONES DEL BANCO
// **********************************

/**
 * Precondición: numJugadores >= 2 y <= 6, jugadores no vacío
 * Postcondición: Se distribuye dinero inicial ($1500) a cada jugador
 */
void bancoDistribuirDineroInicial(Banco& banco, std::vector<Jugador>& jugadores, 
                                   int dineroInicial = 1500) {
    if (jugadores.empty()) {
        std::cout << "❌ Error: No hay jugadores para distribuir dinero" << std::endl;
        return;
    }
    
    for (Jugador& j : jugadores) {
        j.dinero = dineroInicial;
        banco.dineroEnCirculacion += dineroInicial;
    }
    
    std::cout << "🏦 BANCO: Distribuidos $" << dineroInicial << " a " 
              << jugadores.size() << " jugadores" << std::endl;
    std::cout << "💰 Dinero en circulación: $" << banco.dineroEnCirculacion << std::endl;
}

/**
 * Precondición: cantidad > 0, jugador válido, banco tiene fondos
 * Postcondición: El banco otorga dinero al jugador
 */
void bancoOtorgarDinero(Banco& banco, Jugador& jugador, int cantidad, 
                        const std::string& concepto = "") {
    if (cantidad <= 0) {
        std::cout << "❌ El banco no puede otorgar cantidad negativa o cero" << std::endl;
        return;
    }
    
    jugador.dinero += cantidad;
    banco.dineroEnCirculacion += cantidad;
    
    std::cout << "🏦 BANCO otorga $" << cantidad << " a " << jugador.nombre;
    if (!concepto.empty()) std::cout << " por: " << concepto;
    std::cout << std::endl;
    
    // Registrar en historial
    banco.historial.push_back(
        Transaccion("OTORGAR", "BANCO", jugador.nombre, cantidad, concepto, 0)
    );
}

/**
 * Precondición: cantidad > 0, jugador válido
 * Postcondición: El banco cobra dinero al jugador, retorna true si exitoso
 */
bool bancoCobrarDinero(Banco& banco, Jugador& jugador, int cantidad, 
                       const std::string& concepto = "") {
    if (cantidad <= 0) {
        std::cout << "❌ El banco no puede cobrar cantidad negativa o cero" << std::endl;
        return false;
    }
    
    if (jugador.dinero < cantidad) {
        std::cout << "🏦 " << jugador.nombre << " no tiene fondos suficientes para pagar $" 
                  << cantidad << " al BANCO" << std::endl;
        std::cout << "💰 Saldo: $" << jugador.dinero << " (Necesita: $" << cantidad << ")" << std::endl;
        return false;
    }
    
    jugador.dinero -= cantidad;
    banco.dineroEnCirculacion -= cantidad;
    
    std::cout << "🏦 BANCO cobra $" << cantidad << " a " << jugador.nombre;
    if (!concepto.empty()) std::cout << " por: " << concepto;
    std::cout << std::endl;
    
    banco.historial.push_back(
        Transaccion("COBRAR", jugador.nombre, "BANCO", cantidad, concepto, 0)
    );
    
    return true;
}

/**
 * Precondición: Jugadores válidos, cantidad > 0
 * Postcondición: Transfiere dinero entre jugadores
 */
bool bancoTransferencia(Banco& banco, Jugador& pagador, Jugador& receptor, 
                        int cantidad, const std::string& concepto = "") {
    if (cantidad <= 0) {
        std::cout << "❌ No se puede transferir cantidad negativa o cero" << std::endl;
        return false;
    }
    
    if (pagador.dinero < cantidad) {
        std::cout << "🏦 " << pagador.nombre << " no puede pagar $" << cantidad 
                  << " a " << receptor.nombre << std::endl;
        return false;
    }
    
    pagador.dinero -= cantidad;
    receptor.dinero += cantidad;
    
    std::cout << "🏦 TRANSFERENCIA: " << pagador.nombre << " → " << receptor.nombre 
              << " $" << cantidad;
    if (!concepto.empty()) std::cout << " (" << concepto << ")";
    std::cout << std::endl;
    
    banco.historial.push_back(
        Transaccion("TRANSFERENCIA", pagador.nombre, receptor.nombre, cantidad, concepto, 0)
    );
    
    return true;
}

/**
 * Precondición: Jugador válido, está pasando por SALIDA
 * Postcondición: El banco paga $200 al jugador
 */
void bancoPagarSalida(Banco& banco, Jugador& jugador) {
    const int DINERO_SALIDA = 200;
    bancoOtorgarDinero(banco, jugador, DINERO_SALIDA, "Pasar por SALIDA");
}

/**
 * Precondición: Jugador válido, intenta salir de cárcel pagando
 * Postcondición: Cobra multa si tiene fondos, retorna true si exitoso
 */
bool bancoMultaCarcel(Banco& banco, Jugador& jugador) {
    const int MULTA_CARCEL = 50;
    
    if (bancoCobrarDinero(banco, jugador, MULTA_CARCEL, "Multa de cárcel")) {
        jugador.enCarcel = false;
        jugador.turnosCarcel = 0;
        std::cout << "🔓 " << jugador.nombre << " sale de la CÁRCEL" << std::endl;
        return true;
    }
    
    return false;
}

/**
 * Precondición: Parámetros válidos
 * Postcondición: Cobra reparaciones por casas y hoteles
 */
bool bancoReparaciones(Banco& banco, Jugador& jugador, 
                       int costoPorCasa, int costoPorHotel, 
                       int casasActuales, int hotelesActuales) {
    if (costoPorCasa < 0 || costoPorHotel < 0 || casasActuales < 0 || hotelesActuales < 0) {
        std::cout << "❌ Parámetros de reparaciones inválidos" << std::endl;
        return false;
    }
    
    int costoTotal = (casasActuales * costoPorCasa) + (hotelesActuales * costoPorHotel);
    
    std::cout << "🔨 REPARACIONES para " << jugador.nombre << ":" << std::endl;
    std::cout << "  Casas: " << casasActuales << " × $" << costoPorCasa << " = $" 
              << (casasActuales * costoPorCasa) << std::endl;
    std::cout << "  Hoteles: " << hotelesActuales << " × $" << costoPorHotel << " = $" 
              << (hotelesActuales * costoPorHotel) << std::endl;
    std::cout << "  Total: $" << costoTotal << std::endl;
    
    return bancoCobrarDinero(banco, jugador, costoTotal, "Reparaciones generales");
}

/**
 * Precondición: Jugador válido, lista de jugadores válida
 * Postcondición: El jugador paga a todos los demás
 */
bool bancoPagarATodos(Banco& banco, Jugador& pagador, 
                      const std::vector<Jugador>& jugadores, int cantidadPorJugador) {
    int totalAPagar = cantidadPorJugador * (jugadores.size() - 1);
    
    if (pagador.dinero < totalAPagar) {
        std::cout << "🏦 " << pagador.nombre << " no puede pagar $" << cantidadPorJugador 
                  << " a cada jugador" << std::endl;
        return false;
    }
    
    std::cout << "🏦 " << pagador.nombre << " paga $" << cantidadPorJugador << " a todos:" << std::endl;
    
    for (const Jugador& j : jugadores) {
        if (j.nombre != pagador.nombre && !j.estaQuebrado) {
            Jugador& receptor = const_cast<Jugador&>(j);
            bancoTransferencia(banco, pagador, receptor, cantidadPorJugador, "Pago grupal");
        }
    }
    
    return true;
}

/**
 * Precondición: Jugador válido, lista de jugadores válida
 * Postcondición: Todos pagan al jugador especificado
 */
void bancoCobrarDeTodos(Banco& banco, Jugador& receptor, 
                        const std::vector<Jugador>& jugadores, int cantidadPorJugador) {
    std::cout << "🏦 " << receptor.nombre << " cobra $" << cantidadPorJugador << " de todos:" << std::endl;
    
    for (const Jugador& j : jugadores) {
        if (j.nombre != receptor.nombre && !j.estaQuebrado) {
            Jugador& pagador = const_cast<Jugador&>(j);
            if (pagador.dinero >= cantidadPorJugador) {
                bancoTransferencia(banco, pagador, receptor, cantidadPorJugador, "Cobro grupal");
            } else {
                std::cout << "  ⚠️ " << pagador.nombre << " no tiene fondos suficientes" << std::endl;
            }
        }
    }
}

/**
 * Precondición: Jugador válido
 * Postcondición: Declara la quiebra del jugador
 */
void bancoQuiebra(Banco& banco, Jugador& jugador) {
    std::cout << "🏦 QUIEBRA: " << jugador.nombre << " pierde todo su patrimonio" << std::endl;
    std::cout << "  Dinero perdido: $" << jugador.dinero << std::endl;
    std::cout << "  Propiedades perdidas: " << jugador.propiedades.size() << std::endl;
    
    banco.dineroEnCirculacion -= jugador.dinero;
    jugador.dinero = 0;
    jugador.propiedades.clear();
    jugador.estaQuebrado = true;
    
    std::cout << "💀 " << jugador.nombre << " está EN QUIEBRA" << std::endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Muestra resumen financiero de todos los jugadores
 */
void bancoResumenFinanciero(const Banco& banco, const std::vector<Jugador>& jugadores) {
    std::cout << "\n🏦 === RESUMEN FINANCIERO BANCO ===" << std::endl;
    
    int totalDineroEnJuego = 0;
    int jugadoresActivos = 0;
    
    for (const Jugador& j : jugadores) {
        std::cout << "💰 " << j.nombre << ": $" << j.dinero;
        
        if (j.estaQuebrado) {
            std::cout << " [QUEBRADO]";
        } else {
            jugadoresActivos++;
            totalDineroEnJuego += j.dinero;
        }
        
        std::cout << " | Propiedades: " << j.propiedades.size() << std::endl;
    }
    
    std::cout << "\n📊 ESTADÍSTICAS:" << std::endl;
    std::cout << "  Jugadores activos: " << jugadoresActivos << "/" << jugadores.size() << std::endl;
    std::cout << "  Dinero en circulación: $" << banco.dineroEnCirculacion << std::endl;
    std::cout << "  Dinero retenido: $" << banco.dineroRetenido << std::endl;
    std::cout << "  Promedio por jugador: $" << (jugadoresActivos > 0 ? totalDineroEnJuego / jugadoresActivos : 0) << std::endl;
    std::cout << "=================================" << std::endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Muestra historial de últimas transacciones
 */
void bancoMostrarHistorial(const Banco& banco, int ultimasN = 10) {
    std::cout << "\n🏦 === HISTORIAL DE TRANSACCIONES ===" << std::endl;
    
    int inicio = std::max(0, (int)banco.historial.size() - ultimasN);
    
    for (int i = inicio; i < banco.historial.size(); i++) {
        const Transaccion& t = banco.historial[i];
        std::cout << (i + 1) << ". [" << t.tipo << "] " 
                  << t.deJugador << " → " << t.aJugador 
                  << " | $" << t.monto;
        if (!t.concepto.empty()) std::cout << " (" << t.concepto << ")";
        std::cout << std::endl;
    }
    
    std::cout << "=====================================" << std::endl;
}

#endif // BANCO_H_