#include "tablero.h"
#include <iostream>
#include <fstream>
#include "casilla/casilla.h"
#include "casilla/propiedad.h"
#include "casilla/ferrocarril.h"
#include "casilla/servicios.h"
using namespace std;

// ───────────────────────────────────────────────
// Cargar tablero desde archivo .txt
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

        // === PROPIEDAD ===
        if (tipo == "PROPIEDAD") {
            int precio;
            string color;
            int rentas[6];
            int costoCasa;

            archivo >> precio;
            archivo.ignore();
            getline(archivo, color);

            for (int i = 0; i < 6; i++)
                archivo >> rentas[i];
            archivo >> costoCasa;
            archivo.ignore();

            Propiedad* p = new Propiedad(nombreCasilla, precio, color, rentas);
            anxLista(tablero, p);
        }

        // === FERROCARRIL ===
        else if (tipo == "FERROCARRIL") {
            int precio;
            int rentas[4];
            archivo >> precio;
            for (int i = 0; i < 4; i++)
                archivo >> rentas[i];
            archivo.ignore();

            Ferrocarril* f = new Ferrocarril(nombreCasilla, precio, rentas);
            anxLista(tablero, f);
        }

        // === SERVICIO ===
        else if (tipo == "SERVICIO") {
            int precio;
            archivo >> precio;
            archivo.ignore();

            Servicio* s = new Servicio(nombreCasilla, precio);
            anxLista(tablero, s);
        }

        // === CASILLAS SIMPLES ===
        else if (tipo == "SUERTE" || tipo == "COFRE" ||
                 tipo == "SALIDA" || tipo == "CARCEL" ||
                 tipo == "PARQUEADERO" || tipo == "IRCARCEL") {

            Casilla* c = new Casilla(nombreCasilla);
            anxLista(tablero, c);
        }

        // === DESCONOCIDO ===
        else {
            cerr << "⚠️ Tipo de casilla desconocido: " << tipo << endl;
        }

        // Leer hasta el separador ===
        while (getline(archivo, linea)) {
            if (linea == "===") break;
        }
    }

    archivo.close();
    cout << "✅ Tablero cargado exitosamente desde " << nombreArchivo << endl;
    return tablero;
}

// ───────────────────────────────────────────────
// Mostrar el tablero con tipo de cada casilla
// ───────────────────────────────────────────────
void mostrarResumenTablero(const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        cout << "El tablero está vacío." << endl;
        return;
    }

    cout << "=== TABLERO DEL MONOPOLY ===" << endl;
    Casilla* temp = tablero.cabeza;
    int i = 0;
    do {
        cout << "Posición " << i << ": " << temp->getNombre() << endl;
        temp = temp->siguiente;
        i++;
    } while (temp != tablero.cabeza);
    cout << "==============================" << endl;
}

// ───────────────────────────────────────────────
// Función adicional: mostrar estadísticas del tablero
// ───────────────────────────────────────────────
void mostrarEstadisticasTablero(const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        cout << "El tablero está vacío." << endl;
        return;
    }

    int propiedades = 0, ferrocarriles = 0, servicios = 0, especiales = 0;
    
    Casilla* temp = tablero.cabeza;
    do {
        string nombre = temp->getNombre();
        if (nombre.find("RAILROAD") != string::npos || 
            nombre.find("FERROCARRIL") != string::npos) {
            ferrocarriles++;
        }
        else if (nombre == "ELECTRIC COMPANY" || nombre == "WATER WORKS" ||
                 nombre.find("IMPUESTO") != string::npos) {
            servicios++;
        }
        else if (nombre == "SALIDA" || nombre == "CARCEL" || nombre == "PARQUEADERO GRATUITO" ||
                 nombre == "IR A LA CARCEL" || nombre.find("SUERTE") != string::npos ||
                 nombre.find("COMUNIDAD") != string::npos || nombre.find("CHANCE") != string::npos) {
            especiales++;
        }
        else {
            propiedades++;
        }
        
        temp = temp->siguiente;
    } while (temp != tablero.cabeza);
    
    cout << "\n=== ESTADÍSTICAS DEL TABLERO ===" << endl;
    cout << "Propiedades: " << propiedades << endl;
    cout << "Ferrocarriles: " << ferrocarriles << endl;
    cout << "Servicios: " << servicios << endl;
    cout << "Casillas especiales: " << especiales << endl;
    cout << "Total: " << size(tablero) << endl;
    cout << "=================================" << endl;
}
