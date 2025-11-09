#include <iostream>
#include "tablero.h"
using namespace std;

int main() {
    // Crear tablero
    ListaCircular tablero = cargarTableroDesdeArchivo("archivo.txt");

    // Mostrar resumen
    cout << "\n=== TABLERO CREADO ===\n";
    mostrarResumenTablero(tablero);
    cout << "\nTotal casillas: " << size(tablero) << endl;

    // Prueba de activación simple
    Casilla* actual = getCabeza(tablero);
    cout << "\n=== PRUEBA DE ACTIVACIÓN ===\n";
    for (int i = 0; i < 3 && actual != nullptr; i++) {
        cout << "\nJugador cae en: " << actual->getNombre() << endl;
        actual->activar();
        actual = actual->siguiente;
    }

    return 0;
}
