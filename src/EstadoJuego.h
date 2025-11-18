#ifndef ESTADOJUEGO_H_
#define ESTADOJUEGO_H_

#include "pila.h"
#include "CartaJugador/jugador.h"
#include <vector>
#include <string>
#include <iostream>

// **********************************
// Estructuras de datos para estado del juego
// **********************************

struct EstadoJuego {
    std::vector<Jugador> jugadores;
    int turnoActual;
    int rondaActual;
    std::string ultimaAccion;
    
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea un estado de juego inicial
     */
    EstadoJuego() : turnoActual(0), rondaActual(1), ultimaAccion("Inicio del juego") {}
    
    /**
     * Precondición: Los parámetros deben ser válidos
     * Postcondición: Se crea un estado de juego con los datos proporcionados
     */
    EstadoJuego(const std::vector<Jugador>& js, int turno, int ronda, const std::string& accion)
        : jugadores(js), turnoActual(turno), rondaActual(ronda), ultimaAccion(accion) {}
    
    /**
     * Precondición: El estado debe estar inicializado
     * Postcondición: Se muestra la información del estado por consola
     */
    void mostrarInfo() const {
        std::cout << "=== ESTADO DEL JUEGO ===" << std::endl;
        std::cout << "Ronda: " << rondaActual << std::endl;
        std::cout << "Turno actual: " << turnoActual << std::endl;
        std::cout << "Última acción: " << ultimaAccion << std::endl;
        std::cout << "Jugadores: " << jugadores.size() << std::endl;
        
        for (size_t i = 0; i < jugadores.size(); i++) {
            const Jugador& j = jugadores[i];
            std::cout << "  " << (i+1) << ". " << j.nombre 
                      << " - $" << j.dinero 
                      << " - Pos:" << j.posicion;
            if (j.enCarcel) std::cout << " [CÁRCEL]";
            if (j.estaQuebrado) std::cout << " [QUEBRADO]";
            std::cout << std::endl;
        }
        std::cout << "========================" << std::endl;
    }
};

// **********************************
// TAD Historial de Juego con Pila
// **********************************

class HistorialJuego {
private:
    Pila<EstadoJuego> pilaEstados;
    int maxEstados;
    int estadosGuardados;
    
public:
    /**
     * Precondición: maxHist debe ser > 0
     * Postcondición: Se crea un historial vacío con capacidad máxima
     */
    HistorialJuego(int maxHist = 20) : maxEstados(maxHist), estadosGuardados(0) {
        pilaEstados = crearPila<EstadoJuego>();
    }
    
    /**
     * Precondición: Los parámetros del estado deben ser válidos
     * Postcondición: Se guarda el estado actual en la pila
     */
    void guardarEstado(const std::vector<Jugador>& jugadores, int turnoActual, 
                      int rondaActual, const std::string& accion) {
        
        EstadoJuego nuevoEstado(jugadores, turnoActual, rondaActual, accion);
        
        // Controlar límite de estados
        if (estadosGuardados >= maxEstados) {
            std::cout << "⚠️ Límite de estados alcanzado (" << maxEstados << ")" << std::endl;
            // En una implementación más avanzada, se rotaría la pila
        } else {
            pilaEstados = anxPila(pilaEstados, nuevoEstado);
            estadosGuardados++;
        }
        
        std::cout << "💾 Estado guardado: " << accion 
                  << " (" << estadosGuardados << "/" << maxEstados << ")" << std::endl;
    }
    
    /**
     * Precondición: Debe haber al menos un estado guardado
     * Postcondición: Restaura el estado anterior y lo elimina de la pila, retorna true si exitoso
     */
    bool deshacerAccion(std::vector<Jugador>& jugadores, int& turnoActual, 
                       int& rondaActual, std::string& ultimaAccion) {
        
        if (vaciaPila(pilaEstados) || estadosGuardados <= 1) {
            std::cout << "❌ No hay acciones para deshacer" << std::endl;
            return false;
        }
        
        // Eliminar estado actual
        pilaEstados = elimPila(pilaEstados);
        estadosGuardados--;
        
        if (vaciaPila(pilaEstados)) {
            std::cout << "❌ No hay estado anterior al que regresar" << std::endl;
            return false;
        }
        
        // Obtener estado anterior
        EstadoJuego estadoAnterior = infoPila(pilaEstados);
        
        // Restaurar estado
        jugadores = estadoAnterior.jugadores;
        turnoActual = estadoAnterior.turnoActual;
        rondaActual = estadoAnterior.rondaActual;
        ultimaAccion = estadoAnterior.ultimaAccion;
        
        std::cout << "↶ Acción deshecha. Estado: " << ultimaAccion << std::endl;
        return true;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna true si se puede deshacer una acción
     */
    bool puedeDeshacer() const {
        return estadosGuardados > 1;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el número de estados guardados
     */
    int getEstadosGuardados() const {
        return estadosGuardados;
    }
    
    /**
     * Precondición: cantidad debe ser >= 0
     * Postcondición: Muestra los últimos 'cantidad' estados del historial
     */
    void mostrarHistorialReciente(int cantidad = 5) const {
        if (vaciaPila(pilaEstados)) {
            std::cout << "No hay historial disponible" << std::endl;
            return;
        }
        
        std::cout << "\n=== HISTORIAL RECIENTE ===" << std::endl;
        
        // Crear pila temporal para recorrer sin modificar original
        Pila<EstadoJuego> pilaTemp = pilaEstados;
        int contador = 0;
        
        while (!vaciaPila(pilaTemp) && contador < cantidad) {
            EstadoJuego estado = infoPila(pilaTemp);
            std::cout << (contador + 1) << ". [R" << estado.rondaActual 
                      << "-T" << estado.turnoActual << "] " 
                      << estado.ultimaAccion << std::endl;
            
            pilaTemp = elimPila(pilaTemp);
            contador++;
        }
        
        std::cout << "=========================" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Limpia todo el historial, pila queda vacía
     */
    void limpiarHistorial() {
        pilaEstados = crearPila<EstadoJuego>();
        estadosGuardados = 0;
        std::cout << "🗑️ Historial limpiado" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra estadísticas del historial
     */
    void mostrarEstadisticas() const {
        std::cout << "\n=== ESTADÍSTICAS HISTORIAL ===" << std::endl;
        std::cout << "Estados guardados: " << estadosGuardados << "/" << maxEstados << std::endl;
        std::cout << "Puede deshacer: " << (puedeDeshacer() ? "SÍ" : "NO") << std::endl;
        std::cout << "Memoria usada: " << (maxEstados > 0 ? (estadosGuardados * 100 / maxEstados) : 0) << "%" << std::endl;
        std::cout << "==============================" << std::endl;
    }
};

#endif // ESTADOJUEGO_H_