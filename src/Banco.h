#ifndef BANCO_H_
#define BANCO_H_

#include "jugador.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

// **********************************
// Estructura para historial de transacciones
// **********************************
struct Transaccion {
    string tipo;           // "PAGO", "COBRO", "TRANSFERENCIA", etc.
    string deJugador;
    string aJugador;
    int monto;
    string concepto;
    int turno;
    
    Transaccion() : tipo(""), deJugador(""), aJugador(""), monto(0), 
                    concepto(""), turno(0) {}
    
    Transaccion(const string& t, const string& de, 
                const string& a, int m, const string& c, int tu)
        : tipo(t), deJugador(de), aJugador(a), monto(m), concepto(c), turno(tu) {}
};

// **********************************
// TAD BANCO
// **********************************
struct Banco {
    int dineroEnCirculacion;
    int dineroRetenido;          // Propiedades hipotecadas
    vector<Transaccion> historial;
    
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
void bancoDistribuirDineroInicial(Banco& banco, vector<Jugador>& jugadores, 
                                   int dineroInicial = 1500) {
    if (jugadores.empty()) {
        cout << "❌ Error: No hay jugadores para distribuir dinero" << endl;
        return;
    }
    
    for (Jugador& j : jugadores) {
        j.dinero = dineroInicial;
        banco.dineroEnCirculacion += dineroInicial;
    }
    
    cout << "🏦 BANCO: Distribuidos $" << dineroInicial << " a " 
         << jugadores.size() << " jugadores" << endl;
    cout << "💰 Dinero en circulación: $" << banco.dineroEnCirculacion << endl;
}

/**
 * Precondición: cantidad > 0, jugador válido, banco tiene fondos
 * Postcondición: El banco otorga dinero al jugador
 */
void bancoOtorgarDinero(Banco& banco, Jugador& jugador, int cantidad, 
                        const string& concepto = "") {
    if (cantidad <= 0) {
        cout << "❌ El banco no puede otorgar cantidad negativa o cero" << endl;
        return;
    }
    
    jugador.dinero += cantidad;
    banco.dineroEnCirculacion += cantidad;
    
    cout << "🏦 BANCO otorga $" << cantidad << " a " << jugador.nombre;
    if (!concepto.empty()) cout << " por: " << concepto;
    cout << endl;
    
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
                       const string& concepto = "") {
    if (cantidad <= 0) {
        cout << "❌ El banco no puede cobrar cantidad negativa o cero" << endl;
        return false;
    }
    
    if (jugador.dinero < cantidad) {
        cout << "🏦 " << jugador.nombre << " no tiene fondos suficientes para pagar $" 
             << cantidad << " al BANCO" << endl;
        cout << "💰 Saldo: $" << jugador.dinero << " (Necesita: $" << cantidad << ")" << endl;
        return false;
    }
    
    jugador.dinero -= cantidad;
    banco.dineroEnCirculacion -= cantidad;
    
    cout << "🏦 BANCO cobra $" << cantidad << " a " << jugador.nombre;
    if (!concepto.empty()) cout << " por: " << concepto;
    cout << endl;
    
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
                        int cantidad, const string& concepto = "") {
    if (cantidad <= 0) {
        cout << "❌ No se puede transferir cantidad negativa o cero" << endl;
        return false;
    }
    
    if (pagador.dinero < cantidad) {
        cout << "🏦 " << pagador.nombre << " no puede pagar $" << cantidad 
             << " a " << receptor.nombre << endl;
        return false;
    }
    
    pagador.dinero -= cantidad;
    receptor.dinero += cantidad;
    
    cout << "🏦 TRANSFERENCIA: " << pagador.nombre << " → " << receptor.nombre 
         << " $" << cantidad;
    if (!concepto.empty()) cout << " (" << concepto << ")";
    cout << endl;
    
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
        cout << "🔓 " << jugador.nombre << " sale de la CÁRCEL" << endl;
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
        cout << "❌ Parámetros de reparaciones inválidos" << endl;
        return false;
    }
    
    int costoTotal = (casasActuales * costoPorCasa) + (hotelesActuales * costoPorHotel);
    
    cout << "🔨 REPARACIONES para " << jugador.nombre << ":" << endl;
    cout << "  Casas: " << casasActuales << " × $" << costoPorCasa << " = $" 
         << (casasActuales * costoPorCasa) << endl;
    cout << "  Hoteles: " << hotelesActuales << " × $" << costoPorHotel << " = $" 
         << (hotelesActuales * costoPorHotel) << endl;
    cout << "  Total: $" << costoTotal << endl;
    
    return bancoCobrarDinero(banco, jugador, costoTotal, "Reparaciones generales");
}

/**
 * Precondición: Jugador válido, lista de jugadores válida
 * Postcondición: El jugador paga a todos los demás
 */
bool bancoPagarATodos(Banco& banco, Jugador& pagador, 
                      vector<Jugador>& jugadores, int cantidadPorJugador) {
    int totalAPagar = cantidadPorJugador * (jugadores.size() - 1);
    
    if (pagador.dinero < totalAPagar) {
        cout << "🏦 " << pagador.nombre << " no puede pagar $" << cantidadPorJugador 
             << " a cada jugador (necesita $" << totalAPagar << ")" << endl;
        return false;
    }
    
    cout << "🏦 " << pagador.nombre << " paga $" << cantidadPorJugador << " a todos:" << endl;
    
    for (Jugador& j : jugadores) {
        if (j.nombre != pagador.nombre && !j.estaQuebrado) {
            bancoTransferencia(banco, pagador, j, cantidadPorJugador, "Pago grupal");
        }
    }
    
    return true;
}
/**
 * Precondición: Jugador válido, lista de jugadores válida
 * Postcondición: Todos pagan al jugador especificado
 */
void bancoCobrarDeTodos(Banco& banco, Jugador& receptor, 
                        vector<Jugador>& jugadores, int cantidadPorJugador) {
    cout << "🏦 " << receptor.nombre << " cobra $" << cantidadPorJugador << " de todos:" << endl;
    
    for (Jugador& j : jugadores) {
        if (j.nombre != receptor.nombre && !j.estaQuebrado) {
            if (j.dinero >= cantidadPorJugador) {
                bancoTransferencia(banco, j, receptor, cantidadPorJugador, "Cobro grupal");
            } else {
                cout << "  ⚠️ " << j.nombre << " no tiene fondos suficientes" << endl;
            }
        }
    }
}

/**
 * Precondición: Jugador válido
 * Postcondición: Declara la quiebra del jugador
 */
void bancoQuiebra(Banco& banco, Jugador& jugador) {
    cout << "🏦 QUIEBRA: " << jugador.nombre << " pierde todo su patrimonio" << endl;
    cout << "  Dinero perdido: $" << jugador.dinero << endl;
    cout << "  Propiedades perdidas: " << jugador.propiedades.size() << endl;
    
    banco.dineroEnCirculacion -= jugador.dinero;
    jugador.dinero = 0;
    jugador.propiedades.clear();
    jugador.estaQuebrado = true;
    
    cout << "💀 " << jugador.nombre << " está EN QUIEBRA" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Muestra resumen financiero de todos los jugadores
 */
void bancoResumenFinanciero(const Banco& banco, const vector<Jugador>& jugadores) {
    cout << "\n🏦 === RESUMEN FINANCIERO BANCO ===" << endl;
    
    int totalDineroEnJuego = 0;
    int jugadoresActivos = 0;
    
    for (const Jugador& j : jugadores) {
        cout << "💰 " << j.nombre << ": $" << j.dinero;
        
        if (j.estaQuebrado) {
            cout << " [QUEBRADO]";
        } else {
            jugadoresActivos++;
            totalDineroEnJuego += j.dinero;
        }
        
        cout << " | Propiedades: " << j.propiedades.size() << endl;
    }
    
    cout << "\n📊 ESTADÍSTICAS:" << endl;
    cout << "  Jugadores activos: " << jugadoresActivos << "/" << jugadores.size() << endl;
    cout << "  Dinero en circulación: $" << banco.dineroEnCirculacion << endl;
    cout << "  Dinero retenido: $" << banco.dineroRetenido << endl;
    cout << "  Promedio por jugador: $" << (jugadoresActivos > 0 ? totalDineroEnJuego / jugadoresActivos : 0) << endl;
    cout << "=================================" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Muestra historial de últimas transacciones
 */
void bancoMostrarHistorial(const Banco& banco, int ultimasN = 10) {
    cout << "\n🏦 === HISTORIAL DE TRANSACCIONES ===" << endl;
    
    int inicio = max(0, (int)banco.historial.size() - ultimasN);
    
    for (int i = inicio; i < (int)banco.historial.size(); i++) {
        const Transaccion& t = banco.historial[i];
        cout << (i + 1) << ". [" << t.tipo << "] " 
             << t.deJugador << " → " << t.aJugador 
             << " | $" << t.monto;
        if (!t.concepto.empty()) cout << " (" << t.concepto << ")";
        cout << endl;
    }
    
    cout << "=====================================" << endl;
}

#endif // BANCO_H_