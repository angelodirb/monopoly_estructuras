#include "tablero.h"
#include <iostream>
#include <fstream>
#include "casilla.h"
#include "propiedad.h"
#include "ferrocarril.h"
#include "servicios.h"
#include "registroPropiedades.h"
using namespace std;

// ───────────────────────────────────────────────
// Cargar tablero (funcion original)
// ───────────────────────────────────────────────
ListaCircular cargarTableroDesdeArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << nombreArchivo << endl;
        return crearLista();
    }

    ListaCircular tablero = crearLista();
    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        string nombreCasilla = linea;
        string tipo;
        getline(archivo, tipo);

        if (tipo == "PROPIEDAD") {
            int precio;
            int rentas[6];
            int costoCasa;

            archivo >> precio;
            for (int i = 0; i < 6; i++) archivo >> rentas[i];
            archivo >> costoCasa;
            archivo.ignore();

            // Asignar color segun el nombre de la propiedad
            string color = "";
            if (nombreCasilla == "MEDITERRANEAN AVENUE" || nombreCasilla == "BALTIC AVENUE") {
                color = "MARRON";
            }
            else if (nombreCasilla == "ORIENTAL AVENUE" || nombreCasilla == "VERMONT AVENUE" || nombreCasilla == "CONNECTICUT AVENUE") {
                color = "CELESTE";
            }
            else if (nombreCasilla == "ST. CHARLES PLACE" || nombreCasilla == "STATES AVENUE" || nombreCasilla == "VIRGINIA AVENUE") {
                color = "ROSA";
            }
            else if (nombreCasilla == "ST. JAMES PLACE" || nombreCasilla == "TENNESSEE AVENUE" || nombreCasilla == "NEW YORK AVENUE") {
                color = "NARANJA";
            }
            else if (nombreCasilla == "KENTUCKY AVENUE" || nombreCasilla == "INDIANA AVENUE" || nombreCasilla == "ILLINOIS AVENUE") {
                color = "ROJO";
            }
            else if (nombreCasilla == "ATLANTIC AVENUE" || nombreCasilla == "VENTNOR AVENUE" || nombreCasilla == "MARVIN GARDENS") {
                color = "AMARILLO";
            }
            else if (nombreCasilla == "PACIFIC AVENUE" || nombreCasilla == "NORTH CAROLINA AVENUE" || nombreCasilla == "PENNSYLVANIA AVENUE") {
                color = "VERDE";
            }
            else if (nombreCasilla == "PARK PLACE" || nombreCasilla == "BOARDWALK") {
                color = "AZUL";
            }

            Propiedad* p = new Propiedad(nombreCasilla, precio, color, rentas);
            anxLista(tablero, p);
        }
        else if (tipo == "FERROCARRIL") {
            int precio;
            int rentas[4];
            archivo >> precio;
            for (int i = 0; i < 4; i++) archivo >> rentas[i];
            archivo.ignore();

            Ferrocarril* f = new Ferrocarril(nombreCasilla, precio, rentas);
            anxLista(tablero, f);
        }
        else if (tipo == "SERVICIO") {
            int precio;
            archivo >> precio;
            archivo.ignore();

            Servicio* s = new Servicio(nombreCasilla, precio);
            anxLista(tablero, s);
        }
        else {
            Casilla* c = new Casilla(nombreCasilla);
            anxLista(tablero, c);
        }

        while (getline(archivo, linea)) {
            if (linea == "===") break;
        }
    }

    archivo.close();
    return tablero;
}

// ───────────────────────────────────────────────
// Cargar tablero y crear registro
// ───────────────────────────────────────────────
pair<ListaCircular, RegistroPropiedades> cargarTableroConRegistro(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << nombreArchivo << endl;

        ListaCircular vacia = crearLista();
        RegistroPropiedades reg(53);

        return make_pair(vacia, reg);
    }

    ListaCircular tablero = crearLista();
    RegistroPropiedades registro(53);

    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        string nombreCasilla = linea;
        string tipo;
        getline(archivo, tipo);

        if (tipo == "PROPIEDAD") {
            int precio;
            int rentas[6];
            int costoCasa;

            archivo >> precio;
            for (int i = 0; i < 6; i++) archivo >> rentas[i];
            archivo >> costoCasa;
            archivo.ignore();

            // Asignar color segun el nombre de la propiedad
            string color = "";
            if (nombreCasilla == "MEDITERRANEAN AVENUE" || nombreCasilla == "BALTIC AVENUE") {
                color = "MARRON";
            }
            else if (nombreCasilla == "ORIENTAL AVENUE" || nombreCasilla == "VERMONT AVENUE" || nombreCasilla == "CONNECTICUT AVENUE") {
                color = "CELESTE";
            }
            else if (nombreCasilla == "ST. CHARLES PLACE" || nombreCasilla == "STATES AVENUE" || nombreCasilla == "VIRGINIA AVENUE") {
                color = "ROSA";
            }
            else if (nombreCasilla == "ST. JAMES PLACE" || nombreCasilla == "TENNESSEE AVENUE" || nombreCasilla == "NEW YORK AVENUE") {
                color = "NARANJA";
            }
            else if (nombreCasilla == "KENTUCKY AVENUE" || nombreCasilla == "INDIANA AVENUE" || nombreCasilla == "ILLINOIS AVENUE") {
                color = "ROJO";
            }
            else if (nombreCasilla == "ATLANTIC AVENUE" || nombreCasilla == "VENTNOR AVENUE" || nombreCasilla == "MARVIN GARDENS") {
                color = "AMARILLO";
            }
            else if (nombreCasilla == "PACIFIC AVENUE" || nombreCasilla == "NORTH CAROLINA AVENUE" || nombreCasilla == "PENNSYLVANIA AVENUE") {
                color = "VERDE";
            }
            else if (nombreCasilla == "PARK PLACE" || nombreCasilla == "BOARDWALK") {
                color = "AZUL";
            }

            Propiedad* p = new Propiedad(nombreCasilla, precio, color, rentas);
            anxLista(tablero, p);

            registro.registrarPropiedad(nombreCasilla, p);
        }
        else if (tipo == "FERROCARRIL") {
            int precio;
            int rentas[4];
            archivo >> precio;
            for (int i = 0; i < 4; i++) archivo >> rentas[i];
            archivo.ignore();

            Ferrocarril* f = new Ferrocarril(nombreCasilla, precio, rentas);
            anxLista(tablero, f);

            registro.registrarPropiedad(nombreCasilla, f);
        }
        else if (tipo == "SERVICIO") {
            int precio;
            archivo >> precio;
            archivo.ignore();

            Servicio* s = new Servicio(nombreCasilla, precio);
            anxLista(tablero, s);

            registro.registrarPropiedad(nombreCasilla, s);
        }
        else {
            Casilla* c = new Casilla(nombreCasilla);
            anxLista(tablero, c);
        }

        while (getline(archivo, linea)) {
            if (linea == "===") break;
        }
    }

    archivo.close();

    cout << "[OK] Tablero cargado exitosamente\n";
    cout << "   Casillas en tablero: " << size(tablero) << endl;
    cout << "   Propiedades en registro: " << registro.getTotalPropiedades() << endl;

    return make_pair(tablero, registro);
}

// ───────────────────────────────────────────────
// Funciones de visualizacion
// ───────────────────────────────────────────────
void mostrarResumenTablero(const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        cout << "El tablero esta vacio" << endl;
        return;
    }
    
    cout << "\n=== TABLERO DE MONOPOLY ===" << endl;
    cout << "Total de casillas: " << size(tablero) << endl;
    
    Casilla* actual = getCabeza(tablero);
    int contador = 0;
    
    do {
        cout << contador << ". " << actual->getNombre() << endl;
        actual = actual->siguiente;
        contador++;
    } while (actual != getCabeza(tablero));
    
    cout << "===========================" << endl;
}

void mostrarEstadisticasTablero(const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        cout << "El tablero esta vacio" << endl;
        return;
    }
    
    int propiedades = 0;
    int ferrocarriles = 0;
    int servicios = 0;
    int especiales = 0;
    
    Casilla* actual = getCabeza(tablero);
    
    do {
        Propiedad* prop = dynamic_cast<Propiedad*>(actual);
        Ferrocarril* ferro = dynamic_cast<Ferrocarril*>(actual);
        Servicio* serv = dynamic_cast<Servicio*>(actual);
        
        if (prop != nullptr) propiedades++;
        else if (ferro != nullptr) ferrocarriles++;
        else if (serv != nullptr) servicios++;
        else especiales++;
        
        actual = actual->siguiente;
    } while (actual != getCabeza(tablero));
    
    cout << "\n=== ESTADÍSTICAS DEL TABLERO ===" << endl;
    cout << "Total casillas: " << size(tablero) << endl;
    cout << "Propiedades: " << propiedades << endl;
    cout << "Ferrocarriles: " << ferrocarriles << endl;
    cout << "Servicios: " << servicios << endl;
    cout << "Especiales: " << especiales << endl;
    cout << "=================================" << endl;
}

// **********************************
// Implementaciones de funciones auxiliares de propiedades
// **********************************

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

bool registrarPropiedadesDelTablero(RegistroPropiedades& registro, const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        std::cout << "Error: Tablero vacio" << std::endl;
        return false;
    }
    
    std::cout << "Registrando propiedades del tablero..." << std::endl;
    
    Casilla* actual = getCabeza(tablero);
    int registradas = 0;
    
    do {
        std::string nombre = actual->getNombre();
        
        // Solo registrar propiedades comprables
        if (esPropiedadComprable(nombre)) {
            if (registro.registrarPropiedad(nombre, actual)) {
                registradas++;
            }
        }
        
        actual = actual->siguiente;
    } while (actual != getCabeza(tablero));
    
    std::cout << "[OK] " << registradas << " propiedades registradas automaticamente" << std::endl;
    registro.mostrarEstadisticas();
    
    return registradas > 0;
}