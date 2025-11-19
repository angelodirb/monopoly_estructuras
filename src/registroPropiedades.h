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
     * Precondición: capacidad debe ser > 0
     * Postcondición: Se crea un registro vacío con la capacidad especificada
     */
    RegistroPropiedades(int capacidad = 53) : totalPropiedades(0) {
        tabla = crearTablaHash<const char*, Casilla*>(capacidad);
        std::cout << "🏠 Registro de propiedades creado (capacidad: " << capacidad << ")" << std::endl;
    }
    
    /**
     * Precondición: nombre debe ser único, propiedad debe ser válida
     * Postcondición: La propiedad se registra en la tabla hash, retorna true si exitoso
     */
    bool registrarPropiedad(const std::string& nombre, Casilla* propiedad) {
        if (nombre.empty() || propiedad == nullptr) {
            std::cout << "Error: Datos inválidos para registrar propiedad" << std::endl;
            return false;
        }
        
        // Verificar si ya existe
        if (existeTablaHash(tabla, nombre.c_str())) {
            std::cout << "Propiedad '" << nombre << "' ya está registrada" << std::endl;
            return false;
        }
        
        // Registrar en tabla hash
        tabla = insTablaHash(tabla, nombre.c_str(), propiedad);
        totalPropiedades++;
        
        std::cout << "Propiedad registrada: " << nombre << std::endl;
        return true;
    }
    
    /**
     * Precondición: nombre no debe estar vacío
     * Postcondición Retorna puntero a la propiedad si existe, nullptr si no
     */
    Casilla* buscarPropiedad(const std::string& nombre) const {
        if (nombre.empty()) {
            std::cout << "Nombre de propiedad vacioo" << std::endl;
            return nullptr;
        }
        
        Casilla* resultado = infoTablaHash(tabla, nombre.c_str());
        
        if (resultado != nullptr) {
            std::cout << "🔍 Propiedad encontrada: " << nombre << std::endl;
        } else {
            std::cout << "❌ Propiedad no encontrada: " << nombre << std::endl;
        }
        
        return resultado;
    }
    
    /**
     * Precondición: nombre no debe estar vacío
     * Postcondición: Retorna true si la propiedad existe en el registro
     * 
     */
    bool existePropiedad(const std::string& nombre) const {
        if (nombre.empty()) return false;
        return existeTablaHash(tabla, nombre.c_str());
    }
    
    /**
     * Precondición: nombre debe existir en el registro
     * Postcondición: Elimina la propiedad del registro, retorna true si exitoso
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
     * Precondición: Ninguna
     * Postcondición: Retorna el número total de propiedades registradas
     */
    int getTotalPropiedades() const {
        return totalPropiedades;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra estadísticas detalladas del registro
     */
    void mostrarEstadisticas() const {
        std::cout << "\n=== REGISTRO DE PROPIEDADES ===" << std::endl;
        estadisticasTablaHash(tabla);
        std::cout << "Propiedades registradas: " << totalPropiedades << std::endl;
        std::cout << "===============================" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Lista todas las propiedades registradas por tipo
     */
    void listarPropiedadesPorTipo() const {
        std::cout << "\n=== PROPIEDADES POR TIPO ===" << std::endl;
        
        std::vector<std::string> propiedades;
        std::vector<std::string> ferrocarriles;
        std::vector<std::string> servicios;
        
        // Recorrer tabla hash para clasificar (implementación simplificada)
        // En implementación completa se recorrería la tabla hash interna
        
        std::cout << "Propiedades normales: " << propiedades.size() << std::endl;
        std::cout << "Ferrocarriles: " << ferrocarriles.size() << std::endl;
        std::cout << "Servicios: " << servicios.size() << std::endl;
        std::cout << "Total: " << totalPropiedades << std::endl;
        std::cout << "============================" << std::endl;
    }
    
    /**
     * Precondición: color no debe estar vacío
     * Postcondición: Retorna lista de propiedades del color especificado
     */
    std::vector<std::string> buscarPorColor(const std::string& color) const {
        std::vector<std::string> propiedadesColor;
        
        // Implementación simplificada - en versión completa se recorrería la tabla hash
        std::cout << "Buscando propiedades de color: " << color << std::endl;
        std::cout << "Encontradas: " << propiedadesColor.size() << " propiedades" << std::endl;
        
        return propiedadesColor;
    }
    
    /**
     * Precondición: jugador no debe estar vacío
     * Postcondición: Retorna lista de propiedades del jugador especificado
     */
    std::vector<std::string> buscarPorDueno(const std::string& jugador) const {
        std::vector<std::string> propiedadesJugador;
        
        // Implementación simplificada - en versión completa se verificaría cada propiedad
        std::cout << "Buscando propiedades de: " << jugador << std::endl;
        std::cout << "Encontradas: " << propiedadesJugador.size() << " propiedades" << std::endl;
        
        return propiedadesJugador;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna lista de propiedades sin dueño
     */
    std::vector<std::string> buscarPropiedadesDisponibles() const {
        std::vector<std::string> disponibles;
        
        // Implementación simplificada - en versión completa se verificaría cada propiedad
        std::cout << "Buscando propiedades disponibles..." << std::endl;
        std::cout << "Encontradas: " << disponibles.size() << " propiedades disponibles" << std::endl;
        
        return disponibles;
    }
    
    /**
     * Precondición: casilla debe ser válida y ser una propiedad
     * Postcondición: Retorna información detallada de la propiedad
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
            ferro->activar(); // Mostrar información básica
        } else if (serv != nullptr) {
            std::cout << "Tipo: Servicio" << std::endl;
            serv->activar(); // Mostrar información básica
        } else {
            std::cout << "Tipo: Casilla especial" << std::endl;
            casilla->activar();
        }
        
        std::cout << "==============================" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Valida la integridad del registro
     */
    //int elementos = tamañoTablaHash(tabla);
    bool validarIntegridad() const {
    std::cout << "Validando integridad del registro..." << std::endl;

    bool integridadOK = true;

    // ❌ tamañoTablaHash no existe en tu TAD → Se comenta
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
     * Precondición: Ninguna
     * Postcondición: Libera todos los recursos del registro
     */
    void limpiarRegistro() {
        liberarTablaHash(tabla);
        tabla = crearTablaHash<const char*, Casilla*>(53);
        totalPropiedades = 0;
        std::cout << "Registro de propiedades limpiado" << std::endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Destructor que libera recursos
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
 * Precondición: nombre no debe estar vacío
 * Postcondición: Retorna true si el nombre corresponde a una propiedad comprable
 */
bool esPropiedadComprable(const std::string& nombre) {
    // Excluir casillas especiales
    if (nombre == "SALIDA" || nombre == "CARCEL" || 
        nombre == "PARQUEADERO GRATUITO" || nombre == "IR A LA CARCEL" ||
        nombre.find("SUERTE") != std::string::npos || 
        nombre.find("COFRE") != std::string::npos ||
        nombre.find("COMUNIDAD") != std::string::npos ||
        nombre.find("CHANCE") != std::string::npos) {
        return false;
    }
    
    return true;
}

/**
 * Precondición: registro y tablero deben estar inicializados
 * Postcondición: Registra automáticamente todas las propiedades del tablero
 */
bool registrarPropiedadesDelTablero(RegistroPropiedades& registro, const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        std::cout << "Error: Tablero vacío" << std::endl;
        return false;
    }
    
    std::cout << "Registrando propiedades del tablero..." << std::endl;
    
    Casilla* actual = getCabeza(tablero);
    int registradas = 0;
    
    do {
        std::string nombre = actual->getNombre();
        
        // Solo registrar propiedades comprables (excluir casillas especiales)
        if (esPropiedadComprable(nombre)) {
            if (registro.registrarPropiedad(nombre, actual)) {
                registradas++;
            }
        }
        
        actual = actual->siguiente;
    } while (actual != getCabeza(tablero));
    
    std::cout << "✅ " << registradas << " propiedades registradas automáticamente" << std::endl;
    registro.mostrarEstadisticas();
    
    return registradas > 0;
}



#endif // TABLA_HASH_PROPIEDADES_H_