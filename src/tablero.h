#ifndef TABLERO_H_
#define TABLERO_H_

#include <string>
#include "listaCircular.h"

// Prototipo de función pública
ListaCircular cargarTableroDesdeArchivo(const std::string& nombreArchivo);

// Función opcional: mostrar tablero en forma más detallada
void mostrarResumenTablero(const ListaCircular& tablero);

#endif
