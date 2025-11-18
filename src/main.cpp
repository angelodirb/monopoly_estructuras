#include <iostream>
#include <string>
#include <vector>

// Includes de los TADs del proyecto
#include "Juego.h"
#include "EstadoJuego.h"
#include "ColaCartas.h"
#include "RegistroPropiedades.h"
#include "tablero.h"

using namespace std;

// **********************************
// Funciones auxiliares para interfaz
// **********************************

/**
 * Precondición: Ninguna
 * Postcondición: Muestra el menú principal del juego
 */
void mostrarMenuPrincipal() {
    cout << "\n" << string(50, '=') << endl;
    cout << "🎯 MONOPOLY C++ - MENU PRINCIPAL" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Nuevo Juego" << endl;
    cout << "2. Pruebas de TADs individuales" << endl;
    cout << "3. Ver información del proyecto" << endl;
    cout << "4. Salir" << endl;
    cout << string(50, '=') << endl;
    cout << "Seleccione una opción: ";
}

/**
 * Precondición: Ninguna
 * Postcondición: Muestra el menú de pruebas de TADs
 */
void mostrarMenuPruebas() {
    cout << "\n" << string(50, '=') << endl;
    cout << "🧪 PRUEBAS DE TADs" << endl;
    cout << string(50, '=') << endl;
    cout << "1. Probar Tabla Hash de Propiedades" << endl;
    cout << "2. Probar Sistema de Cartas (Colas)" << endl;
    cout << "3. Probar Historial de Estados (Pila)" << endl;
    cout << "4. Probar TAD Jugador" << endl;
    cout << "5. Probar carga de Tablero (Lista Circular)" << endl;
    cout << "6. Volver al menú principal" << endl;
    cout << string(50, '=') << endl;
    cout << "Seleccione una opción: ";
}

/**
 * Precondición: Ninguna
 * Postcondición: Ejecuta pruebas del sistema de tabla hash
 */
void probarTablaHash() {
    cout << "\n🧪 === PRUEBA TABLA HASH DE PROPIEDADES ===" << endl;
    
    RegistroPropiedades registro(53);
    
    // Cargar tablero para prueba
    ListaCircular tablero = cargarTableroDesdeArchivo("Casillas.txt");
    
    if (!vaciaLista(tablero)) {
        cout << "\n--- Registrando propiedades del tablero ---" << endl;
        registrarPropiedadesDelTablero(registro, tablero);
        
        cout << "\n--- Probando búsquedas ---" << endl;
        registro.buscarPropiedad("BOARDWALK");
        registro.buscarPropiedad("READING RAILROAD");
        registro.buscarPropiedad("ELECTRIC COMPANY");
        registro.buscarPropiedad("PROPIEDAD_INEXISTENTE");
        
        cout << "\n--- Mostrando detalles ---" << endl;
        registro.mostrarDetallesPropiedad("PARK PLACE");
        
        cout << "\n--- Estadísticas finales ---" << endl;
        registro.mostrarEstadisticas();
        registro.validarIntegridad();
    } else {
        cout << "❌ Error: No se pudo cargar el tablero para la prueba" << endl;
    }
    
    cout << "\n✅ Prueba de Tabla Hash completada" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Ejecuta pruebas del sistema de cartas
 */
void probarSistemaCartas() {
    cout << "\n🧪 === PRUEBA SISTEMA DE CARTAS (COLAS) ===" << endl;
    
    SistemaCartas sistema;
    
    cout << "\n--- Inicializando sistema ---" << endl;
    if (sistema.inicializar("CartaJugador/cartas.txt")) {
        
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
        
        cout << "\n--- Estado después de usar cartas ---" << endl;
        sistema.mostrarEstado();
        
        cout << "\n--- Próximas cartas ---" << endl;
        sistema.mostrarProximasCartas(2);
        
    } else {
        cout << "❌ Error al inicializar sistema de cartas" << endl;
    }
    
    cout << "\n✅ Prueba de Sistema de Cartas completada" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Ejecuta pruebas del historial de estados
 */
void probarHistorialEstados() {
    cout << "\n🧪 === PRUEBA HISTORIAL DE ESTADOS (PILA) ===" << endl;
    
    HistorialJuego historial(5); // Límite de 5 estados para prueba
    
    // Crear jugadores de prueba
    vector<Jugador> jugadores;
    jugadores.push_back(crearJugador("SOMBRERO"));
    jugadores.push_back(crearJugador("PERRO"));
    
    int turno = 0, ronda = 1;
    string ultimaAccion = "";
    
    cout << "\n--- Guardando estados de prueba ---" << endl;
    historial.guardarEstado(jugadores, turno, ronda, "Inicio del juego");
    
    // Simular algunos cambios
    jugadores[0].posicion = 7;
    jugadores[0].dinero = 1600;
    turno = 1;
    historial.guardarEstado(jugadores, turno, ronda, "SOMBRERO tiró dados");
    
    jugadores[1].posicion = 12;
    jugadores[1].dinero = 1450;
    turno = 0;
    historial.guardarEstado(jugadores, turno, ronda, "PERRO pagó impuesto");
    
    cout << "\n--- Estado actual ---" << endl;
    for (const Jugador& j : jugadores) {
        cout << j.nombre << ": $" << j.dinero << " en casilla " << j.posicion << endl;
    }
    
    cout << "\n--- Historial ---" << endl;
    historial.mostrarHistorialReciente();
    
    cout << "\n--- Probando UNDO ---" << endl;
    if (historial.deshacerAccion(jugadores, turno, ronda, ultimaAccion)) {
        cout << "Estado después de UNDO:" << endl;
        for (const Jugador& j : jugadores) {
            cout << j.nombre << ": $" << j.dinero << " en casilla " << j.posicion << endl;
        }
    }
    
    cout << "\n--- Estadísticas del historial ---" << endl;
    historial.mostrarEstadisticas();
    
    cout << "\n✅ Prueba de Historial de Estados completada" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Ejecuta pruebas del TAD Jugador
 */
void probarTADJugador() {
    cout << "\n🧪 === PRUEBA TAD JUGADOR ===" << endl;
    
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
    
    cout << "\n✅ Prueba de TAD Jugador completada" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Ejecuta pruebas de carga de tablero
 */
void probarTablero() {
    cout << "\n🧪 === PRUEBA CARGA DE TABLERO (LISTA CIRCULAR) ===" << endl;
    
    cout << "\n--- Cargando tablero ---" << endl;
    ListaCircular tablero = cargarTableroDesdeArchivo("Casillas.txt");
    
    if (!vaciaLista(tablero)) {
        cout << "\n--- Resumen del tablero ---" << endl;
        mostrarResumenTablero(tablero);
        
        cout << "\n--- Probando navegación circular ---" << endl;
        Casilla* actual = getCabeza(tablero);
        cout << "Primeras 5 casillas:" << endl;
        for (int i = 0; i < 5 && actual != nullptr; i++) {
            cout << "  " << i << ". " << actual->getNombre() << endl;
            actual = actual->siguiente;
        }
        
        cout << "\n--- Estadísticas ---" << endl;
        mostrarEstadisticasTablero(tablero);
        
    } else {
        cout << "❌ Error: No se pudo cargar el tablero" << endl;
    }
    
    cout << "\n✅ Prueba de Tablero completada" << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Muestra información del proyecto
 */
void mostrarInformacionProyecto() {
    cout << "\n" << string(60, '=') << endl;
    cout << "📋 INFORMACIÓN DEL PROYECTO" << endl;
    cout << string(60, '=') << endl;
    cout << "Proyecto: Monopoly C++" << endl;
    cout << "Materia: Estructuras de Datos" << endl;
    cout << "Profesor: Gerardo M. Sarria M." << endl;
    cout << "Fecha: Octubre 2025" << endl;
    cout << "\n📊 ESTRUCTURAS DE DATOS IMPLEMENTADAS:" << endl;
    cout << "  ✅ Lista Circular (Tablero)" << endl;
    cout << "  ✅ Jerarquía Polimórfica (Casillas)" << endl;
    cout << "  ✅ Tabla Hash (Registro de Propiedades)" << endl;
    cout << "  ✅ Cola (Sistema de Cartas)" << endl;
    cout << "  ✅ Pila (Historial de Estados/Undo)" << endl;
    cout << "  ✅ TAD Jugador" << endl;
    cout << "  ✅ TAD Juego (con funciones del Banco)" << endl;
    cout << "\n🎯 FUNCIONALIDADES:" << endl;
    cout << "  ✅ Interfaz línea de comandos" << endl;
    cout << "  ✅ Sistema de undo (marcha atrás)" << endl;
    cout << "  ✅ Manejo de cartas con colas" << endl;
    cout << "  ✅ Registro eficiente de propiedades" << endl;
    cout << "  ✅ Funciones del banco integradas" << endl;
    cout << "  ✅ Documentación con pre/post condiciones" << endl;
    cout << string(60, '=') << endl;
}

/**
 * Precondición: Ninguna
 * Postcondición: Ejecuta el loop principal del programa
 */
int main() {
    cout << "🎯 ===== MONOPOLY C++ =====" << endl;
    cout << "🎯 Proyecto Estructuras de Datos" << endl;
    cout << "🎯 =========================\n" << endl;
    
    int opcion;
    bool continuar = true;
    
    while (continuar) {
        mostrarMenuPrincipal();
        cin >> opcion;
        
        switch (opcion) {
            case 1: {
                cout << "\n🎮 === INICIANDO NUEVO JUEGO ===" << endl;
                Juego monopoly;
                
                if (monopoly.inicializarJuego()) {
                    cout << "\n¿Comenzar el juego? (s/n): ";
                    char respuesta;
                    cin >> respuesta;
                    
                    if (respuesta == 's' || respuesta == 'S') {
                        monopoly.jugar();
                    } else {
                        cout << "🎮 Juego cancelado" << endl;
                    }
                } else {
                    cout << "❌ Error al inicializar el juego" << endl;
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
                            probarHistorialEstados();
                            break;
                        case 4:
                            probarTADJugador();
                            break;
                        case 5:
                            probarTablero();
                            break;
                        case 6:
                            continuarPruebas = false;
                            break;
                        default:
                            cout << "❌ Opción inválida" << endl;
                            break;
                    }
                    
                    if (continuarPruebas && opcionPrueba >= 1 && opcionPrueba <= 5) {
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
                cout << "\n👋 ¡Gracias por usar Monopoly C++!" << endl;
                break;
                
            default:
                cout << "❌ Opción inválida. Intente nuevamente." << endl;
                break;
        }
    }
    
    return 0;
}