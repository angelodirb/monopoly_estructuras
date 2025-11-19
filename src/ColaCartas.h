#ifndef COLA_CARTAS_H_
#define COLA_CARTAS_H_

#include "cola.h"
#include "cartas.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <iostream>

// **********************************
// TAD Cola de Cartas específico para Monopoly
// **********************************

class ColaCartas {
private:
    Cola<Carta> cola;
    std::string tipo; // "SUERTE" o "COFRE"
    int cartasOriginales;
    
public:
    /**
     * Precondición: tipoCarta debe ser "SUERTE" o "COFRE"
     * Postcondición: Se crea una cola vacía del tipo especificado
     */
    ColaCartas(const std::string& tipoCarta) : tipo(tipoCarta), cartasOriginales(0) {
        cola = crearCola<Carta>();
    }
    
    /**
     * Precondición: El archivo nombreArchivo debe existir y ser accesible
     * Postcondición: Las cartas del tipo correspondiente se cargan en la cola
     */
bool cargarCartasDesdeArchivo(const std::string& nombreArchivo) {
    std::cout << "📂 Intentando abrir: " << nombreArchivo << std::endl;
    
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "❌ Error: No se pudo abrir " << nombreArchivo << std::endl;
        return false;
    }
    
    std::cout << "✅ Archivo abierto. Leyendo cartas de tipo: " << tipo << std::endl;
    
    std::vector<Carta> cartasTemp;
    std::string linea;
    int cartasLeidas = 0;
    
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        
        Carta temp;
        temp.descripcion = linea;
        
        if (!getline(archivo, linea)) break;
        temp.tipo = linea;
        
        if (!getline(archivo, linea)) break;
        temp.accion = linea;
        
        if (!getline(archivo, linea)) break;
        temp.valor = std::stoi(linea);
        temp.valorExtra = 0;
        
        if (getline(archivo, linea) && linea != "===") {
            temp.valorExtra = std::stoi(linea);
            getline(archivo, linea);
        }
        
        if (temp.tipo == tipo) {
            cartasTemp.push_back(temp);
            cartasLeidas++;
            std::cout << "  📝 Carta " << cartasLeidas << ": " << temp.descripcion.substr(0, 30) << "..." << std::endl;
        }
    }
    
    archivo.close();
    
    std::cout << "🔄 Mezclando " << cartasTemp.size() << " cartas..." << std::endl;
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartasTemp.begin(), cartasTemp.end(), g);
    
    for (const Carta& carta : cartasTemp) {
        cola = anxCola(cola, carta);
    }
    
    cartasOriginales = cartasTemp.size();
    std::cout << "✅ Cargadas " << cartasOriginales << " cartas de " << tipo << std::endl;
    return true;
}   
    /**
     * Precondición: La cola no debe estar vacía
     * Postcondición: Retorna la carta del frente y la envía al final (excepto "SALIR_CARCEL")
     */
    Carta sacarCarta() {
        if (vaciaCola(cola)) {
            std::cerr << "❌ Error: No hay cartas en la cola de " << tipo << std::endl;
            return Carta(); // Carta vacía
        }
        
        // Obtener carta del frente
        Carta carta = infoCola(cola);
        
        // Quitar del frente
        cola = elimCola(cola);
        
        // Regresarla al final si NO es carta de "salir de cárcel"
        // (esas se guardan hasta que se usen)
        if (carta.accion != "SALIR_CARCEL") {
            cola = anxCola(cola, carta);
            std::cout << "🎴 Carta de " << tipo << " regresada al final de la cola" << std::endl;
        } else {
            std::cout << "🎴 Carta 'Salir de Cárcel' guardada por el jugador" << std::endl;
        }
        
        return carta;
    }
    
    /**
     * Precondición: carta debe ser válida
     * Postcondición: Devuelve la carta al final de la cola
     */
    void devolverCarta(const Carta& carta) {
        cola = anxCola(cola, carta);
        std::cout << "🎴 Carta devuelta a la cola de " << tipo << ": " << carta.descripcion << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna true si hay cartas en la cola
     */
    bool tieneCartas() const {
        return !vaciaCola(cola);
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el número de cartas en la cola
     */
    int cantidadCartas() const {
        if (vaciaCola(cola)) return 0;
        
        Cola<Carta> colaTemp = cola;
        int count = 0;
        
        while (!vaciaCola(colaTemp)) {
            count++;
            colaTemp = elimCola(colaTemp);
        }
        
        return count;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna el tipo de cola
     */
    std::string getTipo() const {
        return tipo;
    }
    
    /**
     * Precondición: La cola debe tener cartas
     * Postcondición: Mezcla todas las cartas de la cola aleatoriamente
     */
    void mezclarCartas() {
        if (vaciaCola(cola)) {
            std::cout << "⚠️ No hay cartas para mezclar en " << tipo << std::endl;
            return;
        }
        
        // Convertir cola a vector
        std::vector<Carta> cartasTemp;
        while (!vaciaCola(cola)) {
            cartasTemp.push_back(infoCola(cola));
            cola = elimCola(cola);
        }
        
        // Mezclar
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(cartasTemp.begin(), cartasTemp.end(), g);
        
        // Recrear cola
        cola = crearCola<Carta>();
        for (const Carta& carta : cartasTemp) {
            cola = anxCola(cola, carta);
        }
        
        std::cout << "🔄 Cartas de " << tipo << " mezcladas nuevamente" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra información del estado de la cola
     */
    void mostrarInfo() const {
        std::cout << "\n=== COLA DE " << tipo << " ===" << std::endl;
        std::cout << "Cartas disponibles: " << cantidadCartas() << std::endl;
        std::cout << "Cartas originales: " << cartasOriginales << std::endl;
        std::cout << "Estado: " << (tieneCartas() ? "Activa" : "Vacía") << std::endl;
        std::cout << "=====================" << std::endl;
    }
    
    /**
     * Precondición: n debe ser > 0
     * Postcondición: Muestra las próximas n cartas sin modificar la cola
     */
    void mostrarProximasCartas(int n = 3) const {
        if (n <= 0) {
            std::cout << "⚠️ Número de cartas debe ser positivo" << std::endl;
            return;
        }
        
        std::cout << "\n=== PRÓXIMAS " << n << " CARTAS DE " << tipo << " ===" << std::endl;
        
        Cola<Carta> colaTemp = cola;
        
        for (int i = 0; i < n && !vaciaCola(colaTemp); i++) {
            Carta carta = infoCola(colaTemp);
            std::cout << (i + 1) << ". " << carta.descripcion << std::endl;
            std::cout << "   Acción: " << carta.accion << ", Valor: " << carta.valor << std::endl;
            colaTemp = elimCola(colaTemp);
        }
        
        std::cout << "=================================" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Limpia completamente la cola
     */
    void limpiarCola() {
        cola = crearCola<Carta>();
        cartasOriginales = 0;
        std::cout << "🗑️ Cola de " << tipo << " limpiada" << std::endl;
    }
};

// **********************************
// Sistema de Cartas que maneja ambas colas
// **********************************

class SistemaCartas {
private:
    ColaCartas colaSuerte;
    ColaCartas colaCofre;
    
public:
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea el sistema con ambas colas vacías
     */
    SistemaCartas() : colaSuerte("SUERTE"), colaCofre("COFRE") {}
    
    /**
     * Precondición: El archivo archivoCartas debe existir
     * Postcondición: Ambas colas se inicializan con sus cartas respectivas
     */
    bool inicializar(const std::string& archivoCartas) {
        std::cout << "\n🎴 Inicializando sistema de cartas..." << std::endl;
        
        bool suerteOK = colaSuerte.cargarCartasDesdeArchivo(archivoCartas);
        bool cofreOK = colaCofre.cargarCartasDesdeArchivo(archivoCartas);
        
        if (suerteOK && cofreOK) {
            std::cout << "✅ Sistema de cartas listo\n" << std::endl;
            return true;
        } else {
            std::cout << "❌ Error al inicializar sistema de cartas" << std::endl;
            return false;
        }
    }
    
    /**
     * Precondición: La cola de suerte debe tener cartas
     * Postcondición: Retorna una carta de suerte
     */
    Carta sacarCartaSuerte() {
        return colaSuerte.sacarCarta();
    }
    
    /**
     * Precondición: La cola de cofre debe tener cartas
     * Postcondición: Retorna una carta de cofre
     */
    Carta sacarCartaCofre() {
        return colaCofre.sacarCarta();
    }
    
    /**
     * Precondición: carta debe ser válida
     * Postcondición: Devuelve la carta a su cola correspondiente
     */
    void devolverCarta(const Carta& carta) {
        if (carta.tipo == "SUERTE") {
            colaSuerte.devolverCarta(carta);
        } else if (carta.tipo == "COFRE") {
            colaCofre.devolverCarta(carta);
        } else {
            std::cout << "⚠️ Tipo de carta desconocido: " << carta.tipo << std::endl;
        }
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Mezcla todas las cartas de ambas colas
     */
    void mezclarTodasLasCartas() {
        colaSuerte.mezclarCartas();
        colaCofre.mezclarCartas();
        std::cout << "🔄 Todas las cartas mezcladas" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra el estado completo del sistema de cartas
     */
    void mostrarEstado() const {
        std::cout << "\n=== SISTEMA DE CARTAS ===" << std::endl;
        colaSuerte.mostrarInfo();
        colaCofre.mostrarInfo();
        std::cout << "=========================" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna true si ambas colas tienen cartas
     */
    bool cartasDisponibles() const {
        return colaSuerte.tieneCartas() && colaCofre.tieneCartas();
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra las próximas cartas de ambas colas
     */
    void mostrarProximasCartas(int n = 3) const {
        colaSuerte.mostrarProximasCartas(n);
        colaCofre.mostrarProximasCartas(n);
    }
};

#endif // COLA_CARTAS_H_