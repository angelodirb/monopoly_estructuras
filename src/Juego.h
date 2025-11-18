#ifndef JUEGO_H_
#define JUEGO_H_

#include "tablero.h"
#include "Banco.h"
#include "Dado.h"
#include "cola.h"
#include "pila.h"
#include "EstadoJuego.h"
#include "CartaJugador/jugador.h"
#include "CartaJugador/cartas.h"
#include "ColaCartas.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// **********************************
// TAD JUEGO - Sistema principal
// **********************************
class Juego {
private:
    // Estructuras del juego
    ListaCircular tablero;
    Banco banco;
    SistemaCartas sistemaCartas;
    HistorialJuego historial;
    
    
    // Estado del juego
    vector<Jugador> jugadores;
    int turnoActual;
    int rondaActual;
    bool juegoActivo;
    Dado dado;
    
    
    // Control de cárcel
    int turnosEnCarcelActual;
    
    // ===== FUNCIONES PRIVADAS =====
    
    /**
     * Precondición: Tablero cargado
     * Postcondición: Propiedades registradas (usa tabla hash interna del tablero)
     */
    void registrarPropiedades() {
        cout << "✅ Propiedades registradas automáticamente en tabla hash" << endl;
    }
    
    /**
     * Precondición: Archivo de cartas debe existir
     * Postcondición: Cartas cargadas en colas
     */
    void cargarCartas() {
        cout << "🎴 Cargando sistema de cartas..." << endl;
        
        if (sistemaCartas.inicializar("CartaJugador/cartas.txt")) {
            cout << "✅ Sistema de cartas cargado exitosamente" << endl;
            sistemaCartas.mostrarEstado();
        } else {
            cout << "⚠️ Advertencia: Sistema de cartas no disponible" << endl;
        }
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Jugadores configurados con dinero inicial
     */
    void configurarJugadores() {
        int numJugadores;
        
        do {
            cout << "\n👥 ¿Cuántos jugadores? (2-6): ";
            cin >> numJugadores;
            
            if (numJugadores < 2 || numJugadores > 6) {
                cout << "❌ Número inválido. Debe ser entre 2 y 6." << endl;
            }
        } while (numJugadores < 2 || numJugadores > 6);
        
        string nombres[] = {"SOMBRERO", "PERRO", "BARCO", "ZAPATO", "CARRO", "DEDAL"};
        
        cout << "\n👤 Configurando jugadores:" << endl;
        for (int i = 0; i < numJugadores; i++) {
            Jugador j = crearJugador(nombres[i]);
            jugadores.push_back(j);
        }
        
        // El banco distribuye dinero inicial
        bancoDistribuirDineroInicial(banco, jugadores, 1500);
    }
    
    /**
     * Precondición: Jugador válido
     * Postcondición: Retorna la casilla actual del jugador
     */
    Casilla* obtenerCasillaActual(const Jugador& jugador) {
        Casilla* actual = getCabeza(tablero);
        
        for (int i = 0; i < jugador.posicion; i++) {
            actual = actual->siguiente;
        }
        
        return actual;
    }
    
    /**
     * Precondición: Jugador válido, cantidad > 0
     * Postcondición: Procesa el movimiento y la casilla donde cae
     */
    void procesarMovimiento(Jugador& jugador, int casillas) {
        int posicionAnterior = jugador.posicion;
        
        // Calcular nueva posición
        jugador.posicion = (jugador.posicion + casillas) % 40;
        
        cout << "\n🎲 " << jugador.nombre << " se mueve " << casillas 
             << " casillas (de " << posicionAnterior << " a " << jugador.posicion << ")" << endl;
        
        // Verificar si pasó por SALIDA
        if (posicionAnterior > jugador.posicion || 
            (posicionAnterior + casillas >= 40)) {
            cout << "🎯 " << jugador.nombre << " pasó por SALIDA - Recibe $200" << endl;
            bancoPagarSalida(banco, jugador);
        }
    }
    
    /**
     * Precondición: Jugador válido
     * Postcondición: Procesa la acción de la casilla donde cae
     */
    void procesarCasilla(Jugador& jugador) {
        Casilla* casilla = obtenerCasillaActual(jugador);
        
        if (casilla == nullptr) {
            cout << "❌ Error: Casilla nula" << endl;
            return;
        }
        
        string nombreCasilla = casilla->getNombre();
        
        cout << "\n📍 " << jugador.nombre << " cae en: " << nombreCasilla << endl;
        
        // ===== CASILLAS ESPECIALES =====
        
        if (nombreCasilla == "SALIDA") {
            cout << "✅ Estás en SALIDA" << endl;
        }
        else if (nombreCasilla == "CARCEL") {
            if (!jugador.enCarcel) {
                cout << "👮 Estás visitando la cárcel (sin estar preso)" << endl;
            }
        }
        else if (nombreCasilla == "PARQUEADERO GRATUITO") {
            cout << "🅿️ Parqueadero gratuito - Descansa sin pagar" << endl;
        }
        else if (nombreCasilla == "IR A LA CARCEL") {
            cout << "⚠️ ¡IR A LA CÁRCEL! No pases por SALIDA" << endl;
            enviarACarcel(jugador);
        }
        else if (nombreCasilla.find("SUERTE") != string::npos || 
                 nombreCasilla.find("CHANCE") != string::npos) {
            procesarCartaSuerte(jugador);
        }
        else if (nombreCasilla.find("COMUNIDAD") != string::npos || 
                 nombreCasilla.find("COFRE") != string::npos) {
            procesarCartaCofre(jugador);
        }
        
        // ===== PROPIEDADES Y SERVICIOS =====
        else {
            procesarPropiedad(jugador, casilla);
        }
    }
    
    /**
     * Precondición: Jugador válido, casilla es una propiedad
     * Postcondición: Procesa compra o pago de alquiler
     */
    void procesarPropiedad(Jugador& jugador, Casilla* casilla) {
        Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
        Ferrocarril* ferro = dynamic_cast<Ferrocarril*>(casilla);
        Servicio* serv = dynamic_cast<Servicio*>(casilla);
        
        // ===== PROPIEDAD NORMAL =====
        if (prop != nullptr) {
            string duenio = prop->getNombre();  // Placeholder - sería mejor método
            
            cout << "🏠 PROPIEDAD: " << casilla->getNombre() << endl;
            prop->mostrarInfo();
            
            // TODO: Implementar lógica de compra/alquiler
            // if (dueño == "") -> Permitir compra
            // else -> Pagar alquiler
        }
        
        // ===== FERROCARRIL =====
        else if (ferro != nullptr) {
            cout << "🚂 FERROCARRIL: " << casilla->getNombre() << endl;
            ferro->activar();
            
            // TODO: Implementar lógica de ferrocarril
        }
        
        // ===== SERVICIO =====
        else if (serv != nullptr) {
            cout << "⚡ SERVICIO: " << casilla->getNombre() << endl;
            serv->activar();
            
            // TODO: Implementar lógica de servicio
        }
    }
    
    /**
     * Precondición: Jugador válido
     * Postcondición: Procesa una carta de Suerte
     */
    void procesarCartaSuerte(Jugador& jugador) {
        cout << "🎴 ¡Sacaste una carta de SUERTE!" << endl;
        
        Carta carta = sistemaCartas.sacarCartaSuerte();
        mostrarCarta(carta);
        
        // Ejecutar la carta con el estado actual del jugador
        ejecutarCarta(carta, jugador.dinero, jugador.posicion, 
                     jugador.tieneCartaSalirCarcel, 0, 0);
        
        cout << "💰 Dinero actual: $" << jugador.dinero << endl;
    }
    
    /**
     * Precondición: Jugador válido
     * Postcondición: Procesa una carta de Cofre Comunitario
     */
    void procesarCartaCofre(Jugador& jugador) {
        cout << "🎴 ¡Sacaste una carta de COFRE COMUNITARIO!" << endl;
        
        Carta carta = sistemaCartas.sacarCartaCofre();
        mostrarCarta(carta);
        
        // Ejecutar la carta con el estado actual del jugador
        ejecutarCarta(carta, jugador.dinero, jugador.posicion, 
                     jugador.tieneCartaSalirCarcel, 0, 0);
        
        cout << "💰 Dinero actual: $" << jugador.dinero << endl;
    }
    
    /**
     * Precondición: Jugador válido, está en cárcel
     * Postcondición: Intenta salir de cárcel según reglas
     */
    void procesarCárcel(Jugador& jugador) {
        if (!jugador.enCarcel) {
            return;
        }
        
        cout << "\n🔒 " << jugador.nombre << " está en la CÁRCEL" << endl;
        cout << "Turnos: " << jugador.turnosCarcel << "/3" << endl;
        
        // Opción 1: Usar carta
        if (usarCartaSalirCarcel(jugador)) {
            cout << "✅ " << jugador.nombre << " salió gratis con su carta" << endl;
            return;
        }
        
        // Opción 2: Pagar multa
        cout << "\n¿Intentar salir pagando $50? (s/n): ";
        char respuesta;
        cin >> respuesta;
        
        if (respuesta == 's' || respuesta == 'S') {
            if (bancoMultaCarcel(banco, jugador)) {
                cout << "✅ Saliste de cárcel pagando multa" << endl;
                return;
            } else {
                cout << "❌ No tienes dinero para pagar la multa" << endl;
            }
        }
        
        // Opción 3: Esperar turno
        jugador.turnosCarcel++;
        cout << "⏰ Esperas en cárcel (turno " << jugador.turnosCarcel << "/3)" << endl;
        
        if (jugador.turnosCarcel >= 3) {
            jugador.enCarcel = false;
            jugador.turnosCarcel = 0;
            cout << "⏰ 3 turnos completados - Sales automáticamente" << endl;
        }
    }
    
    /**
     * Precondición: Turno válido
     * Postcondición: Ejecuta un turno completo del jugador actual
     */
    void procesarTurno() {
        if (turnoActual >= jugadores.size()) {
            turnoActual = 0;
            rondaActual++;
        }
        
        Jugador& jugadorActual = jugadores[turnoActual];
        
        if (jugadorActual.estaQuebrado) {
            cout << "\n⏭️ " << jugadorActual.nombre << " está quebrado - Turno saltado" << endl;
            turnoActual++;
            return;
        }
        
        // ===== INICIO DEL TURNO =====
        cout << "\n" << string(60, '=') << endl;
        cout << "🎯 RONDA " << rondaActual << " - TURNO DE: " << jugadorActual.nombre << endl;
        cout << string(60, '=') << endl;
        
        mostrarEstadoJugador(jugadorActual);
        
        // ===== PROCESAR CÁRCEL =====
        if (jugadorActual.enCarcel) {
            procesarCárcel(jugadorActual);
            turnoActual++;
            return;  // Turno termina si estaba en cárcel
        }
        
        // ===== LANZAR DADOS =====
        cout << "\n🎲 Presiona Enter para lanzar dados...";
        cin.ignore();
        cin.get();
        
        lanzarDado(dado);
        cout << "SUMA: " << obtenerSuma(dado) << endl;
        
        if (esDoble(dado)) {
            cout << "🔄 ¡DOBLE! Lanzarás de nuevo" << endl;
        }
        
        // ===== MOVIMIENTO =====
        procesarMovimiento(jugadorActual, obtenerSuma(dado));
        
        // ===== PROCESAR CASILLA =====
        procesarCasilla(jugadorActual);
        
        // ===== SIGUIENTE TURNO =====
        turnoActual++;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna true si el juego debe terminar
     */
    bool finDelJuego() const {
        int activos = 0;
        for (const Jugador& j : jugadores) {
            if (!j.estaQuebrado) {
                activos++;
            }
        }
        return activos <= 1;
    }
    
    /**
     * Precondición: Juego finalizado
     * Postcondición: Muestra ganador y estadísticas finales
     */
    void finalizarJuego() {
        cout << "\n" << string(60, '=') << endl;
        cout << "🏆 ¡JUEGO TERMINADO!" << endl;
        cout << string(60, '=') << endl;
        
        // Encontrar ganador
        Jugador* ganador = nullptr;
        int dineroMax = 0;
        
        for (Jugador& j : jugadores) {
            if (!j.estaQuebrado && j.dinero > dineroMax) {
                dineroMax = j.dinero;
                ganador = &j;
            }
        }
        
        if (ganador != nullptr) {
            cout << "\n🎉 ¡GANADOR: " << ganador->nombre << "!" << endl;
            cout << "💰 Dinero final: $" << ganador->dinero << endl;
            cout << "🏠 Propiedades: " << ganador->propiedades.size() << endl;
        }
        
        // Mostrar resumen financiero
        bancoResumenFinanciero(banco, jugadores);
        
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra el estado actual del juego
     */
    void mostrarEstadoActual() const {
        cout << "\n" << string(50, '-') << endl;
        cout << "📊 RONDA " << rondaActual << " | Turno: " << (turnoActual + 1) 
             << "/" << jugadores.size() << endl;
        cout << string(50, '-') << endl;
        
        cout << "\n📋 ESTADO DE JUGADORES:" << endl;
        for (size_t i = 0; i < jugadores.size(); i++) {
            const Jugador& j = jugadores[i];
            cout << (i == turnoActual ? "👉 " : "   ");
            cout << j.nombre << " - $" << j.dinero << " - Pos:" << j.posicion;
            if (j.enCarcel) cout << " [CÁRCEL]";
            if (j.estaQuebrado) cout << " [QUEBRADO]";
            cout << endl;
        }
        cout << string(50, '-') << endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Avanza al siguiente turno
     */
    void avanzarTurno() {
        turnoActual = (turnoActual + 1) % jugadores.size();
        if (turnoActual == 0) {
            rondaActual++;
        }
    }
    
public:
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea un juego con estructuras inicializadas
     */
    Juego() : turnoActual(0), rondaActual(1), juegoActivo(false), turnosEnCarcelActual(0) {
        srand(time(0));
        banco = Banco();
        sistemaCartas = SistemaCartas();
    }
    
    /**
     * Precondición: Archivos de configuración deben existir
     * Postcondición: Inicializa todos los componentes del juego
     */
    bool inicializarJuego() {
        cout << "\n🎯 ===== MONOPOLY C++ =====" << endl;
        cout << "🎯 Proyecto Estructuras de Datos" << endl;
        cout << "🎯 =========================\n" << endl;
        
        // 1. Cargar tablero
        cout << "📋 Cargando tablero..." << endl;
        tablero = cargarTableroDesdeArchivo("Casillas.txt");
        if (vaciaLista(tablero)) {
            cout << "❌ Error cargando tablero" << endl;
            return false;
        }
        cout << "✅ Tablero cargado (" << size(tablero) << " casillas)" << endl;
        
        // 2. Registrar propiedades (tabla hash)
        cout << "\n🏠 Registrando propiedades..." << endl;
        registrarPropiedades();
        
        // 3. Cargar cartas
        cout << "\n🎴 Cargando sistema de cartas..." << endl;
        cargarCartas();
        
        // 4. Configurar jugadores
        cout << "\n👥 Configurando jugadores..." << endl;
        configurarJugadores();
        
        
        juegoActivo = true;
        cout << "\n✅ ¡Juego inicializado correctamente!" << endl;
        return true;
    }
    
    /**
     * Precondición: Juego debe estar inicializado
     * Postcondición: Ejecuta el loop principal del juego
     */
    void jugar() {
        if (!juegoActivo) {
            cout << "❌ Juego no inicializado" << endl;
            return;
        }
        
        cout << "\n🎮 ¡COMENZANDO EL JUEGO!" << endl;
        cout << "Escribe 'undo' durante tu turno para deshacer, 'salir' para terminar\n" << endl;
        
        while (juegoActivo && !finDelJuego()) {
            mostrarEstadoActual();
            
            cout << "\n¿Continuar? (s/n): ";
            char respuesta;
            cin >> respuesta;
            
            if (respuesta == 'n' || respuesta == 'N') {
                cout << "🎮 Juego interrumpido por el usuario" << endl;
                break;
            }
            
            procesarTurno();
            
        }
        
        if (finDelJuego()) {
            finalizarJuego();
        }
    }
    
    /**
     * Precondición: Juego debe estar inicializado
     * Postcondición: Muestra resumen del tablero
     */
    void mostrarTablero() const {
        if (vaciaLista(tablero)) {
            cout << "El tablero está vacío" << endl;
            return;
        }
        
        mostrarResumenTablero(tablero);
        mostrarEstadisticasTablero(tablero);
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna referencia al banco del juego
     */
    Banco& obtenerBanco() {
        return banco;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna referencia a los jugadores del juego
     */
    vector<Jugador>& obtenerJugadores() {
        return jugadores;
    }
    
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra información del estado actual
     */
    void mostrarInfo() const {
        cout << "\n=== INFORMACIÓN DEL JUEGO ===" << endl;
        cout << "Ronda: " << rondaActual << endl;
        cout << "Turno actual: " << turnoActual + 1 << "/" << jugadores.size() << endl;
        cout << "Juego activo: " << (juegoActivo ? "SÍ" : "NO") << endl;
        cout << "Jugadores: " << jugadores.size() << endl;
        cout << "==============================" << endl;
    }
};

#endif // JUEGO_H_