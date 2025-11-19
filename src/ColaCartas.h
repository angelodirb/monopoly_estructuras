#ifndef COLA_CARTAS_H_
#define COLA_CARTAS_H_

#include "cola.h"
#include "cartas.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

// **********************************
// TAD Cola de Cartas especifico para Monopoly
// **********************************

class ColaCartas {
private:
    Cola<Carta> cola;
    std::string tipo; // "SUERTE" o "COFRE"
    int cartasOriginales;
    
public:
    /**
     * Precondicion: tipoCarta debe ser "SUERTE" o "COFRE"
     * Postcondicon: Se crea una cola vacia del tipo especificado
     */
    ColaCartas(const std::string& tipoCarta) : tipo(tipoCarta), cartasOriginales(0) {
        cola = crearCola<Carta>();
    }
    
    /**
     * Precondicion: El archivo nombreArchivo debe existir y ser accesible
     * Postcondicion: Las cartas del tipo correspondiente se cargan en la cola
     */
bool cargarCartasDesdeArchivo(const std::string& nombreArchivo) {
    std::cout << "[ARCHIVO] Intentando abrir: " << nombreArchivo << std::endl;
    
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] Error: No se pudo abrir " << nombreArchivo << std::endl;
        return false;
    }
    
    std::cout << "[OK] Archivo abierto. Leyendo cartas de tipo: " << tipo << std::endl;
    
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
            std::cout << "  [NOTA] Carta " << cartasLeidas << ": " << temp.descripcion.substr(0, 30) << "..." << std::endl;
        }
    }
    
    archivo.close();

    if (cartasTemp.empty()) {
        std::cout << "[AVISO] No se encontraron cartas de tipo " << tipo << std::endl;
        return false;
    }

    // Mezclar las cartas antes de cargarlas
    std::mt19937 g(static_cast<unsigned int>(std::time(nullptr)));
    std::shuffle(cartasTemp.begin(), cartasTemp.end(), g);

    // Cargar cartas en la cola
    for (const Carta& carta : cartasTemp) {
        cola = anxCola(cola, carta);
    }

    cartasOriginales = cartasTemp.size();
    std::cout << "[OK] Sistema caro " << cartasOriginales << " cartas de " << tipo << " en la cola" << std::endl;
    return true;
}   
    /**
     * Precondicon: La cola no debe estar vacia
     * Postcondicon: Retorna la carta del frente y la envia al final (excepto "SALIR_CARCEL")
     */
    Carta sacarCarta() {
        if (vaciaCola(cola)) {
            std::cerr << "[ERROR] Error: No hay cartas en la cola de " << tipo << std::endl;
            return Carta(); // Carta vacia
        }
        
        // Obtener carta del frente
        Carta carta = infoCola(cola);
        
        // Quitar del frente
        cola = elimCola(cola);
        
        // Regresarla al final si NO es carta de "salir de carcel"
        // (esas se guardan hasta que se usen)
        if (carta.accion != "SALIR_CARCEL") {
            cola = anxCola(cola, carta);
            std::cout << "[CARTA] Carta de " << tipo << " regresada al final de la cola" << std::endl;
        } else {
            std::cout << "[CARTA] Carta 'Salir de Carcel' guardada por el jugador" << std::endl;
        }
        
        return carta;
    }
    
    /**
     * Precondicon: carta debe ser valida
     * Postcondicon: Devuelve la carta al final de la cola
     */
    void devolverCarta(const Carta& carta) {
        cola = anxCola(cola, carta);
        std::cout << "[CARTA] Carta devuelta a la cola de " << tipo << ": " << carta.descripcion << std::endl;
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Retorna true si hay cartas en la cola
     */
    bool tieneCartas() const {
        return !vaciaCola(cola);
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Retorna el numero de cartas en la cola
     */
    int cantidadCartas() const {
        if (vaciaCola(cola)) return 0;

        // Recorrer la cola sin eliminar nodos
        Nodo<Carta>* actual = cola;
        int count = 0;

        while (actual != NULL) {
            count++;
            actual = actual->sig;
        }

        return count;
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Retorna el tipo de cola
     */
    std::string getTipo() const {
        return tipo;
    }
    
    /**
     * Precondicon: La cola debe tener cartas
     * Postcondicon: Mezcla todas las cartas de la cola aleatoriamente
     */
    void mezclarCartas() {
        if (vaciaCola(cola)) {
            std::cout << "[AVISO] No hay cartas para mezclar en " << tipo << std::endl;
            return;
        }
        
        // Convertir cola a vector
        std::vector<Carta> cartasTemp;
        while (!vaciaCola(cola)) {
            cartasTemp.push_back(infoCola(cola));
            cola = elimCola(cola);
        }

        // Mezclar (usar time() para compatibilidad Windows/MinGW)
        std::mt19937 g(static_cast<unsigned int>(std::time(nullptr)));
        std::shuffle(cartasTemp.begin(), cartasTemp.end(), g);

        // Recrear cola
        cola = crearCola<Carta>();
        for (const Carta& carta : cartasTemp) {
            cola = anxCola(cola, carta);
        }

        std::cout << "[MEZCLAR] Cartas de " << tipo << " mezcladas nuevamente" << std::endl;
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Muestra informacion del estado de la cola
     */
    void mostrarInfo() const {
        std::cout << "\n=== COLA DE " << tipo << " ===" << std::endl;
        std::cout << "Cartas disponibles: " << cantidadCartas() << std::endl;
        std::cout << "Cartas originales: " << cartasOriginales << std::endl;
        std::cout << "Estado: " << (tieneCartas() ? "Activa" : "Vacia") << std::endl;
        std::cout << "=====================" << std::endl;
    }
    
    /**
     * Precondicon: n debe ser > 0
     * Postcondicon: Muestra las proximas n cartas sin modificar la cola
     */
    void mostrarProximasCartas(int n = 3) const {
        if (n <= 0) {
            std::cout << "[AVISO] Numero de cartas debe ser positivo" << std::endl;
            return;
        }

        std::cout << "\n=== PROXIMAS " << n << " CARTAS DE " << tipo << " ===" << std::endl;

        // Recorrer la cola sin eliminar nodos
        Nodo<Carta>* actual = cola;

        for (int i = 0; i < n && actual != NULL; i++) {
            Carta carta = actual->dato;
            std::cout << (i + 1) << ". " << carta.descripcion << std::endl;
            std::cout << "   Accon: " << carta.accion << ", Valor: " << carta.valor << std::endl;
            actual = actual->sig;
        }

        std::cout << "=================================" << std::endl;
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Limpia completamente la cola
     */
    void limpiarCola() {
        cola = crearCola<Carta>();
        cartasOriginales = 0;
        std::cout << "[LIMPIAR] Cola de " << tipo << " limpiada" << std::endl;
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
     * Precondicon: Ninguna
     * Postcondicon: Se crea el sistema con ambas colas vacias
     */
    SistemaCartas() : colaSuerte("SUERTE"), colaCofre("COFRE") {}
    
    /**
     * Precondicon: El archivo archivoCartas debe existir
     * Postcondicon: Ambas colas se inicializan con sus cartas respectivas
     */
    bool inicializar(const std::string& archivoCartas) {
        std::cout << "\n[CARTA] Inicializando sistema de cartas..." << std::endl;
        
        bool suerteOK = colaSuerte.cargarCartasDesdeArchivo(archivoCartas);
        bool cofreOK = colaCofre.cargarCartasDesdeArchivo(archivoCartas);
        
        if (suerteOK && cofreOK) {
            std::cout << "[OK] Sistema de cartas listo\n" << std::endl;
            return true;
        } else {
            std::cout << "[ERROR] Error al inicializar sistema de cartas" << std::endl;
            return false;
        }
    }
    
    /**
     * Precondicon: La cola de suerte debe tener cartas
     * Postcondicon: Retorna una carta de suerte
     */
    Carta sacarCartaSuerte() {
        return colaSuerte.sacarCarta();
    }
    
    /**
     * Precondicon: La cola de cofre debe tener cartas
     * Postcondicon: Retorna una carta de cofre
     */
    Carta sacarCartaCofre() {
        return colaCofre.sacarCarta();
    }
    
    /**
     * Precondicon: carta debe ser valida
     * Postcondicon: Devuelve la carta a su cola correspondiente
     */
    void devolverCarta(const Carta& carta) {
        if (carta.tipo == "SUERTE") {
            colaSuerte.devolverCarta(carta);
        } else if (carta.tipo == "COFRE") {
            colaCofre.devolverCarta(carta);
        } else {
            std::cout << "[AVISO] Tipo de carta desconocido: " << carta.tipo << std::endl;
        }
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Mezcla todas las cartas de ambas colas
     */
    void mezclarTodasLasCartas() {
        colaSuerte.mezclarCartas();
        colaCofre.mezclarCartas();
        std::cout << "[MEZCLAR] Todas las cartas mezcladas" << std::endl;
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Muestra el estado completo del sistema de cartas
     */
    void mostrarEstado() const {
        std::cout << "\n=== SISTEMA DE CARTAS ===" << std::endl;
        colaSuerte.mostrarInfo();
        colaCofre.mostrarInfo();
        std::cout << "=========================" << std::endl;
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Retorna true si ambas colas tienen cartas
     */
    bool cartasDisponibles() const {
        return colaSuerte.tieneCartas() && colaCofre.tieneCartas();
    }
    
    /**
     * Precondicon: Ninguna
     * Postcondicon: Muestra las proximas cartas de ambas colas
     */
    void mostrarProximasCartas(int n = 3) const {
        colaSuerte.mostrarProximasCartas(n);
        colaCofre.mostrarProximasCartas(n);
    }
};

#endif // COLA_CARTAS_H_
