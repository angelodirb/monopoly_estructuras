#ifndef TABLERO_H_
#define TABLERO_H_

#include <string>
#include <utility>  // Para std::pair
#include "listaCircular.h"
#include "registroPropiedades.h"

using namespace std;

// Función original (mantiene compatibilidad)
ListaCircular cargarTableroDesdeArchivo(const std::string& nombreArchivo);

// NUEVA: Función que carga tablero Y crea registro de propiedades
pair<ListaCircular, RegistroPropiedades> cargarTableroConRegistro(const std::string& nombreArchivo);

// Funciones de visualización
void mostrarResumenTablero(const ListaCircular& tablero);
void mostrarEstadisticasTablero(const ListaCircular& tablero);

#endif