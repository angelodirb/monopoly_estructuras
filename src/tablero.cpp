#include "tablero.h"
#include <iostream>
#include <fstream>
#include "casilla.h"
#include "propiedad.h"
#include "ferrocarril.h"
#include "servicios.h"
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

        while (getline(archivo, linea)) {
            if (linea == "===") break;
        }
    }

    archivo.close();
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

    Casilla* temp = tablero.cabeza;
    int i = 1;
    do {
        cout << i++ << ". " << temp->getNombre() << endl;
        temp = temp->siguiente;
    } while (temp != tablero.cabeza);
}
