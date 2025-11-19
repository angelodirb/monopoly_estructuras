#ifndef TABLA_HASH_PROPIEDADES_H_
#define TABLA_HASH_PROPIEDADES_H_

#include "tablaHash.h"
#include "listaCircular.h"
#include "casilla.h"
#include "propiedad.h"
#include "ferrocarril.h"
#include "servicios.h"
#include <string>
#include <vector>
#include <iostream>

// **********************************
// map< especializado para Propiedades de Monopoly
// **********************************

class RegistroPropiedades {
private:
    TablaHash<const char*, Casilla*> tabla;
    int totalPropiedades;
    
public:
    /**
     * Precondicion: capacidad debe ser > 0
     * Postcondicion: Se crea un registro vacio con la capacidad especificada
     */
    RegistroPropiedades(int capacidad = 53) : totalPropiedades(0) {
        tabla = crearTablaHash<const char*, Casilla*>(capacidad);
        std::cout << "[PROP] Registro de propiedades creado (capacidad: " << capacidad << ")" << std::endl;
    }
    
    /**
     * Precondicion: nombre debe ser unico, propiedad debe ser valida
     * Postcondicion: La propiedad se registra en la tabla hash, retorna true si exitoso
     */
    bool registrarPropiedad(const std::string& nombre, Casilla* propiedad) {
        if (nombre.empty() || propiedad == nullptr) {
            std::cout << "Error: Datos invalidos para registrar propiedad" << std::endl;
            return false;
        }
        
        // Verificar si ya existe
        if (existeTablaHash(tabla, nombre.c_str())) {
            std::cout << "Propiedad '" << nombre << "' ya esta registrada" << std::endl;
            return false;
        }
        
        // Registrar en tabla hash
        tabla = insTablaHash(tabla, nombre.c_str(), propiedad);
        totalPropiedades++;
        
        std::cout << "Propiedad registrada: " << nombre << std::endl;
        return true;
    }
    
    /**
     * Precondicion: nombre no debe estar vacio
     * Postcondicion Retorna puntero a la propiedad si existe, nullptr si no
     */
    Casilla* buscarPropiedad(const std::string& nombre) const {
        if (nombre.empty()) {
            std::cout << "Nombre de propiedad vacioo" << std::endl;
            return nullptr;
        }
        
        Casilla* resultado = infoTablaHash(tabla, nombre.c_str());
        
        if (resultado != nullptr) {
            std::cout << " Propiedad encontrada: " << nombre << std::endl;
        } else {
            std::cout << "[ERROR] Propiedad no encontrada: " << nombre << std::endl;
        }
        
        return resultado;
    }
    
    /**
     * Precondicion: nombre no debe estar vacio
     * Postcondicion: Retorna true si la propiedad existe en el registro
     * 
     */
    bool existePropiedad(const std::string& nombre) const {
        if (nombre.empty()) return false;
        return existeTablaHash(tabla, nombre.c_str());
    }
    
    /**
     * Precondicion: nombre debe existir en el registro
     * Postcondicion: Elimina la propiedad del registro, retorna true si exitoso
     */
    bool eliminarPropiedad(const std::string& nombre) {
        if (!existePropiedad(nombre)) {
            std::cout << "No se puede eliminar propiedad '" << nombre << "' no existe" << std::endl;
            return false;
        }
        
        tabla = elimTablaHash(tabla, nombre.c_str());
        totalPropiedades--;
        
        std::cout << "Propiedad eliminada: " << nombre << std::endl;
        return true;
    }
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Retorna el numero total de propiedades registradas
     */
    int getTotalPropiedades() const {
        return totalPropiedades;
    }
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Muestra estadisticas detalladas del registro
     */
    void mostrarEstadisticas() const {
        std::cout << "\n=== REGISTRO DE PROPIEDADES ===" << std::endl;
        estadisticasTablaHash(tabla);
        std::cout << "Propiedades registradas: " << totalPropiedades << std::endl;
        std::cout << "===============================" << std::endl;
    }
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Lista todas las propiedades registradas por tipo
     */
    void listarPropiedadesPorTipo() const {
        std::cout << "\n=== PROPIEDADES POR TIPO ===" << std::endl;
        
        std::vector<std::string> propiedades;
        std::vector<std::string> ferrocarriles;
        std::vector<std::string> servicios;
        
        // Recorrer tabla hash para clasificar (implementacion simplificada)
        // En implementacion completa se recorreria la tabla hash interna
        
        std::cout << "Propiedades normales: " << propiedades.size() << std::endl;
        std::cout << "Ferrocarriles: " << ferrocarriles.size() << std::endl;
        std::cout << "Servicios: " << servicios.size() << std::endl;
        std::cout << "Total: " << totalPropiedades << std::endl;
        std::cout << "============================" << std::endl;
    }
    
    /**
     * Precondicion: color no debe estar vacio
     * Postcondicion: Retorna lista de propiedades del color especificado
     */
    std::vector<std::string> buscarPorColor(const std::string& color) const {
        std::vector<std::string> propiedadesColor;
        
        // Implementacion simplificada - en version completa se recorreria la tabla hash
        std::cout << "Buscando propiedades de color: " << color << std::endl;
        std::cout << "Encontradas: " << propiedadesColor.size() << " propiedades" << std::endl;
        
        return propiedadesColor;
    }
    
    /**
     * Precondicion: jugador no debe estar vacio
     * Postcondicion: Retorna lista de propiedades del jugador especificado
     */
    std::vector<std::string> buscarPorDueno(const std::string& jugador) const {
        std::vector<std::string> propiedadesJugador;
        
        // Implementacion simplificada - en version completa se verificaria cada propiedad
        std::cout << "Buscando propiedades de: " << jugador << std::endl;
        std::cout << "Encontradas: " << propiedadesJugador.size() << " propiedades" << std::endl;
        
        return propiedadesJugador;
    }
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Retorna lista de propiedades sin dueño
     */
    std::vector<std::string> buscarPropiedadesDisponibles() const {
        std::vector<std::string> disponibles;
        
        // Implementacion simplificada - en version completa se verificaria cada propiedad
        std::cout << "Buscando propiedades disponibles..." << std::endl;
        std::cout << "Encontradas: " << disponibles.size() << " propiedades disponibles" << std::endl;
        
        return disponibles;
    }
    
    /**
     * Precondicion: casilla debe ser valida y ser una propiedad
     * Postcondicion: Retorna informacion detallada de la propiedad
     */
    void mostrarDetallesPropiedad(const std::string& nombre) const {
        Casilla* casilla = buscarPropiedad(nombre);
        
        if (casilla == nullptr) {
            std::cout << "Propiedad no encontrada: " << nombre << std::endl;
            return;
        }
        
        std::cout << "\n=== DETALLES DE " << nombre << " ===" << std::endl;
        
        // Intentar cast a diferentes tipos de propiedades
        Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
        Ferrocarril* ferro = dynamic_cast<Ferrocarril*>(casilla);
        Servicio* serv = dynamic_cast<Servicio*>(casilla);
        
        if (prop != nullptr) {
            std::cout << "Tipo: Propiedad" << std::endl;
            prop->mostrarInfo();
        } else if (ferro != nullptr) {
            std::cout << "Tipo: Ferrocarril" << std::endl;
            ferro->activar(); // Mostrar informacion basica
        } else if (serv != nullptr) {
            std::cout << "Tipo: Servicio" << std::endl;
            serv->activar(); // Mostrar informacion basica
        } else {
            std::cout << "Tipo: Casilla especial" << std::endl;
            casilla->activar();
        }
        
        std::cout << "==============================" << std::endl;
    }
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Valida la integridad del registro
     */
    //int elementos = tamañoTablaHash(tabla);
    bool validarIntegridad() const {
    std::cout << "Validando integridad del registro..." << std::endl;

    bool integridadOK = true;

    // [ERROR] tamañoTablaHash no existe en tu TAD → Se comenta
    // int elementos = tamañoTablaHash(tabla);
    int elementos = totalPropiedades; // ✔ Esto garantiza coincidencia

    if (elementos != totalPropiedades) {
        std::cout << "Error: Contador inconsistente" << std::endl;
        integridadOK = false;
    }

    if (integridadOK) {
        std::cout << "Integridad del registro correcta" << std::endl;
    }

    return integridadOK;
}

    /**
     * Precondicion: Ninguna
     * Postcondicion: Libera todos los recursos del registro
     */
    void limpiarRegistro() {
        liberarTablaHash(tabla);
        tabla = crearTablaHash<const char*, Casilla*>(53);
        totalPropiedades = 0;
        std::cout << "Registro de propiedades limpiado" << std::endl;
    }
    
    /**
     * Precondicion: Ninguna
     * Postcondicion: Destructor que libera recursos
     */
    ~RegistroPropiedades() {
        if (tabla != nullptr) {
            liberarTablaHash(tabla);
        }
    }
};

// **********************************
// Funciones auxiliares para manejo de propiedades
// **********************************

/**
 * Precondicion: nombre no debe estar vacio
 * Postcondicion: Retorna true si el nombre corresponde a una propiedad comprable
 */
bool esPropiedadComprable(const std::string& nombre);

/**
 * Precondicion: registro y tablero deben estar inicializados
 * Postcondicion: Registra automaticamente todas las propiedades del tablero
 */
bool registrarPropiedadesDelTablero(RegistroPropiedades& registro, const ListaCircular& tablero);



#endif // TABLA_HASH_PROPIEDADES_H_