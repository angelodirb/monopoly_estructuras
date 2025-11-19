#include <iostream>
#include <string>
#include <vector>


#include "Juego.h"
#include "ColaCartas.h"
#include "registroPropiedades.h"
#include "tablero.h"

using namespace std;

// **********************************
// Funciones auxiliares para interfaz
// **********************************

/**
 * Precondicion: Ninguna
 * Postcondicion: Muestra el menu principal del juego
 */
void mostrarMenuPrincipal() {
    cout << "\n" << string(50, '=') << endl;
    cout << ">>> MONOPOLY C++ - MENU PRINCIPAL" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Nuevo Juego" << endl;
    cout << "2. Pruebas de TADs individuales" << endl;
    cout << "3. Ver informacion del proyecto" << endl;
    cout << "4. Salir" << endl;
    cout << string(50, '=') << endl;
    cout << "Seleccione una opcion: ";
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Muestra el menu de pruebas de TADs
 */
void mostrarMenuPruebas() {
    cout << "\n" << string(50, '=') << endl;
    cout << "[TEST] PRUEBAS DE TADs" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Probar Tabla Hash de Propiedades" << endl;
    cout << "2. Probar Sistema de Cartas (Colas)" << endl;
    cout << "3. Probar TAD Jugador" << endl;
    cout << "4. Probar carga de Tablero (Lista Circular)" << endl;
    cout << "5. Volver al menu principal" << endl;
    cout << string(50, '=') << endl;
    cout << "Seleccione una opcion: ";
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Ejecuta pruebas del sistema de tabla hash
 */
void probarTablaHash() {
    cout << "\n[TEST] === PRUEBA TABLA HASH DE PROPIEDADES ===" << endl;
    
    RegistroPropiedades registro(53);
    
    // Cargar tablero para prueba
    ListaCircular tablero = cargarTableroDesdeArchivo("Casillas.txt");
    
    if (!vaciaLista(tablero)) {
        cout << "\n--- Registrando propiedades del tablero ---" << endl;
        registrarPropiedadesDelTablero(registro, tablero);
        
        cout << "\n--- Probando busquedas ---" << endl;
        registro.buscarPropiedad("BOARDWALK");
        registro.buscarPropiedad("READING RAILROAD");
        registro.buscarPropiedad("ELECTRIC COMPANY");
        registro.buscarPropiedad("PROPIEDAD_INEXISTENTE");
        
        cout << "\n--- Mostrando detalles ---" << endl;
        registro.mostrarDetallesPropiedad("PARK PLACE");
        
        cout << "\n--- Estadisticas finales ---" << endl;
        registro.mostrarEstadisticas();
        registro.validarIntegridad();
    } else {
        cout << "[ERROR] Error: No se pudo cargar el tablero para la prueba" << endl;
    }
    
    cout << "\n[OK] Prueba de Tabla Hash completada" << endl;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Ejecuta pruebas del sistema de cartas
 */
void probarSistemaCartas() {    
    cout << "\n[TEST] === PRUEBA SISTEMA DE CARTAS (COLAS) ===" << endl;
    
    SistemaCartas sistema;
    
    cout << "\n--- Inicializando sistema ---" << endl;
    if (sistema.inicializar("src/cartas.txt")) {
        
        cout << "\n--- Estado inicial ---" << endl;
        sistema.mostrarEstado();
        
        cout << "\n--- Sacando cartas de prueba ---" << endl;
        Carta cartaSuerte = sistema.sacarCartaSuerte();
        mostrarCarta(cartaSuerte);
        
        Carta cartaCofre = sistema.sacarCartaCofre();
        mostrarCarta(cartaCofre);
        
        cout << "\n--- Devolviendo carta (simulando carta usada) ---" << endl;
        if (cartaSuerte.accion != "SALIR_CARCEL") {
            sistema.devolverCarta(cartaSuerte);
        }
        
        cout << "\n--- Estado despues de usar cartas ---" << endl;
        sistema.mostrarEstado();
        
        cout << "\n--- Proximas cartas ---" << endl;
        sistema.mostrarProximasCartas(2);
        
    } else {
        cout << "[ERROR] Error al inicializar sistema de cartas" << endl;
    }
    
    cout << "\n[OK] Prueba de Sistema de Cartas completada" << endl;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Ejecuta pruebas del TAD Jugador
 */
void probarTADJugador() {
    cout << "\n[TEST] === PRUEBA TAD JUGADOR ===" << endl;
    
    cout << "\n--- Creando jugadores ---" << endl;
    Jugador j1 = crearJugador("SOMBRERO");
    Jugador j2 = crearJugador("BARCO");
    
    cout << "\n--- Estado inicial ---" << endl;
    cout << j1.nombre << ": $" << j1.dinero << " en casilla " << j1.posicion << endl;
    cout << j2.nombre << ": $" << j2.dinero << " en casilla " << j2.posicion << endl;
    
    cout << "\n--- Probando movimiento ---" << endl;
    moverJugador(j1, 7);
    moverJugador(j1, 35); // Dar vuelta completa
    
    cout << "\n--- Probando transacciones ---" << endl;
    agregarDinero(j1, 300);
    retirarDinero(j1, 150);
    pagarA(j1, j2, 200);
    
    cout << "\n--- Probando compra de propiedades ---" << endl;
    comprarPropiedad(j1, "Boardwalk", 400);
    comprarPropiedad(j1, "Park Place", 350);
    
    cout << "\n--- Estado final ---" << endl;
    cout << j1.nombre << ": $" << j1.dinero << " en casilla " << j1.posicion 
         << " con " << j1.propiedades.size() << " propiedades" << endl;
    cout << j2.nombre << ": $" << j2.dinero << " en casilla " << j2.posicion 
         << " con " << j2.propiedades.size() << " propiedades" << endl;
    
    cout << "\n[OK] Prueba de TAD Jugador completada" << endl;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Ejecuta pruebas de carga de tablero
 */
void probarTablero() {
    cout << "\n[TEST] === PRUEBA CARGA DE TABLERO (LISTA CIRCULAR) ===" << endl;
    
    cout << "\n--- Cargando tablero ---" << endl;
    ListaCircular tablero = cargarTableroDesdeArchivo("src/Casillas.txt");
    
    if (!vaciaLista(tablero)) {
        cout << "\n--- Resumen del tablero ---" << endl;
        mostrarResumenTablero(tablero);
        
        cout << "\n--- Probando navegacion circular ---" << endl;
        Casilla* actual = getCabeza(tablero);
        cout << "Primeras 5 casillas:" << endl;
        for (int i = 0; i < 5 && actual != nullptr; i++) {
            cout << "  " << i << ". " << actual->getNombre() << endl;
            actual = actual->siguiente;
        }
        
        cout << "\n--- Estadisticas ---" << endl;
        mostrarEstadisticasTablero(tablero);
        
    } else {
        cout << "[ERROR] Error: No se pudo cargar el tablero" << endl;
    }
    
    cout << "\n[OK] Prueba de Tablero completada" << endl;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Muestra informacion del proyecto
 */
void mostrarInformacionProyecto() {
    cout << "\n" << string(60, '=') << endl;
    cout << "[INFO] INFORMACION DEL PROYECTO" << endl;
    cout << string(60, '=') << endl;
    cout << "Proyecto: Monopoly C++" << endl;
    cout << "Materia: Estructuras de Datos" << endl;
    cout << "Profesor: Gerardo M. Sarria M." << endl;
    cout << "Fecha: Octubre 2025" << endl;
    cout << "\n[DATOS] ESTRUCTURAS DE DATOS IMPLEMENTADAS:" << endl;
    cout << "  [OK] Lista Circular (Tablero)" << endl;
    cout << "  [OK] Jerarquia Polimorfica (Casillas)" << endl;
    cout << "  [OK] Tabla Hash (Registro de Propiedades)" << endl;
    cout << "  [OK] Cola (Sistema de Cartas)" << endl;
    // Eliminada la mencion a Pila/Undo
    cout << "  [OK] TAD Jugador" << endl;
    cout << "  [OK] TAD Juego (con funciones del Banco)" << endl;
    cout << "\n>>> FUNCIONALIDADES:" << endl;
    cout << "  [OK] Interfaz linea de comandos" << endl;
    cout << "  [OK] Manejo de cartas con colas" << endl;
    cout << "  [OK] Registro eficiente de propiedades" << endl;
    cout << "  [OK] Funciones del banco integradas" << endl;
    cout << "  [OK] Documentacion con pre/post condiciones" << endl;
    cout << string(60, '=') << endl;
}

/**
 * Precondicion: Ninguna
 * Postcondicion: Ejecuta el loop principal del programa
 */
int main() {
    cout << ">>> ===== MONOPOLY C++ =====" << endl;
    cout << ">>> Proyecto Estructuras de Datos" << endl;
    cout << ">>> =========================\n" << endl;
    
    int opcion;
    bool continuar = true;
    
    while (continuar) {
        mostrarMenuPrincipal();
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                cout << "\n[JUEGO] === INICIANDO NUEVO JUEGO ===" << endl;
                Juego monopoly;
                
                if (monopoly.inicializarJuego()) {
                    cout << "\nComenzar el juego? (s/n): ";
                    char respuesta;
                    cin >> respuesta;

                    if (respuesta == 's' || respuesta == 'S') {
                        monopoly.jugar();
                    } else {
                        cout << "[JUEGO] Juego cancelado" << endl;
                    }
                } else {
                    cout << "[ERROR] Error al inicializar el juego" << endl;
                }
                break;
            }
            
            case 2: {
                int opcionPrueba;
                bool continuarPruebas = true;
                
                while (continuarPruebas) {
                    mostrarMenuPruebas();
                    cin >> opcionPrueba;
                    
                    switch (opcionPrueba) {
                        case 1:
                            probarTablaHash();
                            break;
                        case 2:
                            probarSistemaCartas();
                            break;
                        case 3:
                            probarTADJugador();
                            break;
                        case 4:
                            probarTablero();
                            break;
                        case 5:
                            continuarPruebas = false;
                            break;
                        default:
                            cout << "[ERROR] Opcion invalida" << endl;
                            break;
                    }
                    
                    if (continuarPruebas && opcionPrueba >= 1 && opcionPrueba <= 4) {
                        cout << "\nPresione Enter para continuar...";
                        cin.ignore();
                        cin.get();
                    }
                }
                break;
            }
            
            case 3:
                mostrarInformacionProyecto();
                cout << "\nPresione Enter para continuar...";
                cin.ignore();
                cin.get();
                break;
                
            case 4:
                continuar = false;
                cout << "\n[ADIOS] Gracias por usar Monopoly C++!" << endl;
                break;
                
            default:
                cout << "[ERROR] Opcion invalida. Intente nuevamente." << endl;
                break;
        }
    }
    
    return 0;
}