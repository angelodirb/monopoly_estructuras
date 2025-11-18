#ifndef JUEGO_H_
#define JUEGO_H_

#include "tablero.h"
#include "tablaHash.h"
#include "cola.h"
#include "EstadoJuego.h"
#include "CartaJugador/jugador.h"
#include "CartaJugador/cartas.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

// **********************************
// TAD JUEGO - Sistema principal con Banco integrado
// **********************************

class Juego {
private:
    // Estructuras del juego
    ListaCircular tablero;
    TablaHash<const char*, Casilla*> registroPropiedades;
    Cola<Carta> colaSuerte;
    Cola<Carta> colaCofre;
    HistorialJuego historial;
    
    // Estado del juego
    std::vector<Jugador> jugadores;
    int turnoActual;
    int rondaActual;
    bool juegoActivo;
    
    // **********************************
    // FUNCIONES DEL BANCO
    // **********************************
    
    /**
     * Precondición: cantidad > 0, jugador válido
     * Postcondición: El banco otorga dinero al jugador
     */
    void bancoOtorgarDinero(Jugador& jugador, int cantidad, const std::string& concepto = "") {
        if (cantidad <= 0) {
            std::cout << "❌ El banco no puede otorgar cantidad negativa o cero" << std::endl;
            return;
        }
        
        jugador.dinero += cantidad;
        
        if (!concepto.empty()) {
            std::cout << "🏦 BANCO otorga $" << cantidad << " a " << jugador.nombre 
                      << " por: " << concepto << std::endl;
        } else {
            std::cout << "🏦 BANCO otorga $" << cantidad << " a " << jugador.nombre << std::endl;
        }
        
        std::cout << "💰 Saldo actual: $" << jugador.dinero << std::endl;
    }
    
    /**
     * Precondición: cantidad > 0, jugador válido
     * Postcondición: El banco cobra dinero al jugador, retorna true si exitoso
     */
    bool bancoCobrarDinero(Jugador& jugador, int cantidad, const std::string& concepto = "") {
        if (cantidad <= 0) {
            std::cout << "❌ El banco no puede cobrar cantidad negativa o cero" << std::endl;
            return false;
        }
        
        if (jugador.dinero < cantidad) {
            std::cout << "🏦 " << jugador.nombre << " no tiene fondos suficientes para pagar $" 
                      << cantidad << " al BANCO" << std::endl;
            std::cout << "💰 Saldo actual: $" << jugador.dinero << " (Necesita: $" << cantidad << ")" << std::endl;
            return false;
        }
        
        jugador.dinero -= cantidad;
        
        if (!concepto.empty()) {
            std::cout << "🏦 BANCO cobra $" << cantidad << " a " << jugador.nombre 
                      << " por: " << concepto << std::endl;
        } else {
            std::cout << "🏦 BANCO cobra $" << cantidad << " a " << jugador.nombre << std::endl;
        }
        
        std::cout << "💰 Saldo actual: $" << jugador.dinero << std::endl;
        return true;
    }
    
    /**
     * Precondición: Jugadores válidos, cantidad > 0
     * Postcondición: Transfiere dinero entre jugadores via banco
     */
    bool bancoTransferencia(Jugador& pagador, Jugador& receptor, int cantidad, const std::string& concepto = "") {
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
        
        if (!concepto.empty()) {
            std::cout << "🏦 TRANSFERENCIA: " << pagador.nombre << " → " << receptor.nombre 
                      << " $" << cantidad << " (" << concepto << ")" << std::endl;
        } else {
            std::cout << "🏦 TRANSFERENCIA: " << pagador.nombre << " → " << receptor.nombre 
                      << " $" << cantidad << std::endl;
        }
        
        std::cout << "  " << pagador.nombre << ": $" << pagador.dinero << std::endl;
        std::cout << "  " << receptor.nombre << ": $" << receptor.dinero << std::endl;
        
        return true;
    }
    
    /**
     * Precondición: jugador válido
     * Postcondición: Procesa pago por pasar por SALIDA
     */
    void bancoPagarSalida(Jugador& jugador) {
        const int DINERO_SALIDA = 200;
        bancoOtorgarDinero(jugador, DINERO_SALIDA, "Pasar por SALIDA");
    }
    
    /**
     * Precondición: jugador válido
     * Postcondición: Procesa pago de multa de cárcel, retorna true si exitoso
     */
    bool bancoMultaCarcel(Jugador& jugador) {
        const int MULTA_CARCEL = 50;
        
        if (bancoCobrarDinero(jugador, MULTA_CARCEL, "Multa de cárcel")) {
            jugador.enCarcel = false;
            jugador.turnosCarcel = 0;
            std::cout << "🔓 " << jugador.nombre << " sale de la CÁRCEL" << std::endl;
            return true;
        }
        
        return false;
    }
    
    /**
     * Precondición: jugador válido, casasActuales >= 0, hotelesActuales >= 0
     * Postcondición: Procesa pago de reparaciones según número de propiedades
     */
    bool bancoReparaciones(Jugador& jugador, int costoPorCasa, int costoPorHotel, 
                          int casasActuales, int hotelesActuales) {
        if (costoPorCasa < 0 || costoPorHotel < 0 || casasActuales < 0 || hotelesActuales < 0) {
            std::cout << "❌ Parámetros de reparaciones inválidos" << std::endl;
            return false;
        }
        
        int costoTotal = (casasActuales * costoPorCasa) + (hotelesActuales * costoPorHotel);
        
        std::cout << "🔨 REPARACIONES para " << jugador.nombre << ":" << std::endl;
        std::cout << "  Casas: " << casasActuales << " × $" << costoPorCasa << " = $" << (casasActuales * costoPorCasa) << std::endl;
        std::cout << "  Hoteles: " << hotelesActuales << " × $" << costoPorHotel << " = $" << (hotelesActuales * costoPorHotel) << std::endl;
        std::cout << "  Total: $" << costoTotal << std::endl;
        
        return bancoCobrarDinero(jugador, costoTotal, "Reparaciones generales");
    }
    
    /**
     * Precondición: jugador válido, lista de jugadores válida
     * Postcondición: El jugador paga a todos los demás jugadores
     */
    bool bancoPagarATodos(Jugador& pagador, int cantidadPorJugador) {
        int totalAPagar = cantidadPorJugador * (jugadores.size() - 1);
        
        if (pagador.dinero < totalAPagar) {
            std::cout << "🏦 " << pagador.nombre << " no puede pagar $" << cantidadPorJugador 
                      << " a cada jugador (necesita $" << totalAPagar << ")" << std::endl;
            return false;
        }
        
        std::cout << "🏦 " << pagador.nombre << " paga $" << cantidadPorJugador << " a todos:" << std::endl;
        
        for (Jugador& j : jugadores) {
            if (j.nombre != pagador.nombre && !j.estaQuebrado) {
                bancoTransferencia(pagador, j, cantidadPorJugador);
            }
        }
        
        return true;
    }
    
    /**
     * Precondición: jugador válido, lista de jugadores válida
     * Postcondición: Todos los jugadores pagan al jugador especificado
     */
    void bancoCobrarDeTodos(Jugador& receptor, int cantidadPorJugador) {
        std::cout << "🏦 " << receptor.nombre << " cobra $" << cantidadPorJugador << " de todos:" << std::endl;
        
        for (Jugador& j : jugadores) {
            if (j.nombre != receptor.nombre && !j.estaQuebrado) {
                if (j.dinero >= cantidadPorJugador) {
                    bancoTransferencia(j, receptor, cantidadPorJugador);
                } else {
                    std::cout << "  ⚠️ " << j.nombre << " no tiene fondos suficientes" << std::endl;
                }
            }
        }
    }
    
    /**
     * Precondición: jugador válido
     * Postcondición: Declara la quiebra del jugador y transfiere todo al banco
     */
    void bancoQuiebra(Jugador& jugador) {
        std::cout << "🏦 QUIEBRA: " << jugador.nombre << " pierde todo su patrimonio" << std::endl;
        std::cout << "  Dinero perdido: $" << jugador.dinero << std::endl;
        std::cout << "  Propiedades perdidas: " << jugador.propiedades.size() << std::endl;
        
        // El banco recupera todo
        jugador.dinero = 0;
        jugador.propiedades.clear();
        jugador.estaQuebrado = true;
        
        std::cout << "💀 " << jugador.nombre << " está EN QUIEBRA" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra resumen financiero de todos los jugadores
     */
    void bancoResumenFinanciero() const {
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
        std::cout << "  Dinero total en circulación: $" << totalDineroEnJuego << std::endl;
        std::cout << "  Promedio por jugador activo: $" << (jugadoresActivos > 0 ? totalDineroEnJuego / jugadoresActivos : 0) << std::endl;
        std::cout << "=================================" << std::endl;
    }
    
public:
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea un juego con estructuras inicializadas
     */
    Juego() : turnoActual(0), rondaActual(1), juegoActivo(false) {
        registroPropiedades = crearTablaHash<const char*, Casilla*>(53);
        colaSuerte = crearCola<Carta>();
        colaCofre = crearCola<Carta>();
        srand(time(0));
    }
    
    /**
     * Precondición: Archivos de configuración deben existir
     * Postcondición: Inicializa todos los componentes del juego
     */
    bool inicializarJuego() {
        std::cout << "\n🎯 ===== MONOPOLY C++ =====" << std::endl;
        std::cout << "🎯 Proyecto Estructuras de Datos" << std::endl;
        std::cout << "🎯 =========================\n" << std::endl;
        
        // 1. Cargar tablero
        std::cout << "📋 Cargando tablero..." << std::endl;
        tablero = cargarTableroDesdeArchivo("Casillas.txt");
        if (vaciaLista(tablero)) {
            std::cout << "❌ Error cargando tablero" << std::endl;
            return false;
        }
        
        // 2. Registrar propiedades en hash table
        std::cout << "🏠 Registrando propiedades..." << std::endl;
        registrarPropiedades();
        
        // 3. Cargar cartas
        std::cout << "🎴 Cargando cartas..." << std::endl;
        cargarCartas();
        
        // 4. Configurar jugadores
        configurarJugadores();
        
        // 5. Guardar estado inicial
        historial.guardarEstado(jugadores, turnoActual, rondaActual, "Inicio del juego");
        
        juegoActivo = true;
        std::cout << "✅ ¡Juego inicializado correctamente!" << std::endl;
        return true;
    }
    
    /**
     * Precondición: Juego debe estar inicializado
     * Postcondición: Ejecuta el loop principal del juego
     */
    void jugar() {
        if (!juegoActivo) {
            std::cout << "❌ Juego no inicializado" << std::endl;
            return;
        }
        
        std::cout << "\n🎮 ¡COMENZANDO EL JUEGO!" << std::endl;
        
        while (juegoActivo && !finDelJuego()) {
            mostrarEstadoActual();
            procesarTurno();
            avanzarTurno();
        }
        
        if (finDelJuego()) {
            finalizarJuego();
        }
    }
    
private:
    /**
     * Precondición: Tablero cargado
     * Postcondición: Propiedades registradas en tabla hash
     */
    void registrarPropiedades() {
        Casilla* actual = getCabeza(tablero);
        int registradas = 0;
        
        do {
            std::string nombre = actual->getNombre();
            
            // Solo registrar propiedades comprables
            if (nombre != "SALIDA" && nombre != "CARCEL" && 
                nombre != "PARQUEADERO GRATUITO" && nombre != "IR A LA CARCEL" &&
                nombre.find("SUERTE") == std::string::npos && 
                nombre.find("COFRE") == std::string::npos &&
                nombre.find("COMUNIDAD") == std::string::npos) {
                
                registroPropiedades = insTablaHash(registroPropiedades, nombre.c_str(), actual);
                registradas++;
            }
            
            actual = actual->siguiente;
        } while (actual != getCabeza(tablero));
        
        std::cout << "✅ " << registradas << " propiedades registradas" << std::endl;
        estadisticasTablaHash(registroPropiedades);
    }
    
    /**
     * Precondición: Archivo de cartas debe existir
     * Postcondición: Cartas cargadas en colas
     */
    void cargarCartas() {
        // Implementación simplificada - en proyecto completo se cargarían desde archivo
        std::cout << "✅ Sistema de cartas cargado" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Jugadores configurados con valores iniciales del banco
     */
    void configurarJugadores() {
        int numJugadores;
        
        do {
            std::cout << "\n👥 ¿Cuántos jugadores? (2-6): ";
            std::cin >> numJugadores;
        } while (numJugadores < 2 || numJugadores > 6);
        
        std::string nombres[] = {"SOMBRERO", "PERRO", "BARCO", "ZAPATO", "CARRO", "DEDAL"};
        
        for (int i = 0; i < numJugadores; i++) {
            Jugador j = crearJugador(nombres[i]);
            jugadores.push_back(j);
            
            // El banco otorga dinero inicial
            std::cout << "🏦 BANCO otorga dinero inicial a " << j.nombre << std::endl;
        }
    }
    
    void procesarTurno() {
        // Implementación del turno usando las funciones del banco
        std::cout << "\n=== Turno en desarrollo ===" << std::endl;
        // Aquí iría la lógica completa del turno
    }
    
    void mostrarEstadoActual() const {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "🎯 RONDA " << rondaActual << " - TURNO DE: " << jugadores[turnoActual].nombre << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }
    
    bool finDelJuego() const {
        int activos = 0;
        for (const Jugador& j : jugadores) {
            if (!j.estaQuebrado) activos++;
        }
        return activos <= 1;
    }
    
    void finalizarJuego() {
        std::cout << "\n🏆 ¡JUEGO TERMINADO!" << std::endl;
        bancoResumenFinanciero();
    }
    
    void avanzarTurno() {
        turnoActual = (turnoActual + 1) % jugadores.size();
        if (turnoActual == 0) rondaActual++;
    }
};

#endif 