#include "tablero.h"
#include <iostream>
#include <fstream>
#include "casilla/casilla.h"
#include "casilla/propiedad.h"
#include "casilla/ferrocarril.h"
#include "casilla/servicios.h"
#include "registroPropiedades.h"  // NUEVO
using namespace std;

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
