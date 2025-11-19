#include "tablero.h"
#include <iostream>
#include <fstream>
#include "casilla.h"
#include "propiedad.h"
#include "ferrocarril.h"
#include "servicios.h"
#include "registroPropiedades.h"  // NUEVO
using namespace std;

// ───────────────────────────────────────────────
// Cargar tablero y crear registro
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
            string color;
            int rentas[6];
            int costoCasa;

            archivo >> precio;
            archivo.ignore();
            getline(archivo, color);

            for (int i = 0; i < 6; i++) archivo >> rentas[i];
            archivo >> costoCasa;
            archivo.ignore();

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

pair<ListaCircular, RegistroPropiedades> cargarTableroConRegistro(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << nombreArchivo << endl;

        ListaCircular vacia = crearLista();
        RegistroPropiedades reg(53);

        return make_pair(vacia, reg);
    }

    ListaCircular tablero = crearLista();
    RegistroPropiedades registro(53);   // ✔ OBJETO, no función

    string linea;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        string nombreCasilla = linea;
        string tipo;
        getline(archivo, tipo);

        if (tipo == "PROPIEDAD") {
            int precio;
            string color;
            int rentas[6];
            int costoCasa;

            archivo >> precio;
            archivo.ignore();
            getline(archivo, color);

            for (int i = 0; i < 6; i++) archivo >> rentas[i];
            archivo >> costoCasa;
            archivo.ignore();

            Propiedad* p = new Propiedad(nombreCasilla, precio, color, rentas);
            anxLista(tablero, p);

            registro.registrarPropiedad(nombreCasilla, p);  // ✔ CORRECTO
        }
        else if (tipo == "FERROCARRIL") {
            int precio;
            int rentas[4];
            archivo >> precio;
            for (int i = 0; i < 4; i++) archivo >> rentas[i];
            archivo.ignore();

            Ferrocarril* f = new Ferrocarril(nombreCasilla, precio, rentas);
            anxLista(tablero, f);

            registro.registrarPropiedad(nombreCasilla, f);  // ✔ CORRECTO
        }
        else if (tipo == "SERVICIO") {
            int precio;
            archivo >> precio;
            archivo.ignore();

            Servicio* s = new Servicio(nombreCasilla, precio);
            anxLista(tablero, s);

            registro.registrarPropiedad(nombreCasilla, s);  // ✔ CORRECTO
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

    cout << "✅ Tablero cargado exitosamente\n";
    cout << "   Casillas en tablero: " << size(tablero) << endl;
    cout << "   Propiedades en registro: " << registro.getTotalPropiedades() << endl;  // ✔

    return make_pair(tablero, registro);
}

void mostrarResumenTablero(const ListaCircular& tablero) {
    if (vaciaLista(tablero)) {
        cout << "El tablero está vacío" << endl;
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
        cout << "El tablero está vacío" << endl;
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
