// prueba_jugador.cpp
#include <iostream>
#include <vector>
#include "jugador.h"

using namespace std;

void imprimirSeparador()
{
    cout << "\n========================================\n" << endl;
}

void mostrarEstadoJugador(const Jugador& j)
{
    cout << "\n--- Estado de " << j.nombre << " ---" << endl;
    cout << "Dinero: $" << j.dinero << endl;
    cout << "Posición: Casilla " << j.posicion << endl;
    cout << "Propiedades: " << j.propiedades.size() << endl;
    
    if (j.propiedades.size() > 0)
    {
        cout << "  Lista: ";
        for (size_t i = 0; i < j.propiedades.size(); i++)
        {
            cout << j.propiedades[i];
            if (i < j.propiedades.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    
    cout << "En cárcel: " << (j.enCarcel ? "Sí" : "No") << endl;
    if (j.enCarcel)
    {
        cout << "Turnos en cárcel: " << j.turnosCarcel << endl;
    }
    cout << "Tiene carta libertad: " << (j.tieneCarta ? "Sí" : "No") << endl;
    cout << "Estado: " << (j.estaQuebrado ? "QUEBRADO" : "Activo") << endl;
}

int main()
{
    cout << "====================================" << endl;
    cout << "  PRUEBA DEL TAD JUGADOR - MONOPOLY" << endl;
    cout << "====================================" << endl;
    
    // ===== CREAR JUGADORES =====
    imprimirSeparador();
    cout << "1. CREANDO JUGADORES" << endl;
    
    Jugador j1 = crearJugador("SOMBRERO");
    Jugador j2 = crearJugador("PERRO");
    Jugador j3 = crearJugador("BARCO");
    
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    mostrarEstadoJugador(j3);
    
    // ===== OPERACIONES DE MOVIMIENTO =====
    imprimirSeparador();
    cout << "2. PRUEBAS DE MOVIMIENTO" << endl;
    
    cout << "\n--- Movimiento básico ---" << endl;
    moverJugador(j1, 7);
    mostrarEstadoJugador(j1);
    
    cout << "\n--- Dar vuelta completa (pasar por salida) ---" << endl;
    moverJugador(j1, 35);
    mostrarEstadoJugador(j1);
    
    cout << "\n--- Ir directamente a una casilla ---" << endl;
    irACasilla(j2, 15);
    mostrarEstadoJugador(j2);
    
    cout << "\n--- Enviar a la cárcel ---" << endl;
    enviarACarcel(j3);
    mostrarEstadoJugador(j3);
    
    // ===== OPERACIONES FINANCIERAS BÁSICAS =====
    imprimirSeparador();
    cout << "3. PRUEBAS FINANCIERAS BÁSICAS" << endl;
    
    cout << "\n--- Agregar dinero ---" << endl;
    agregarDinero(j1, 500);
    mostrarEstadoJugador(j1);
    
    cout << "\n--- Retirar dinero (exitoso) ---" << endl;
    bool exito = retirarDinero(j1, 300);
    cout << "Operación " << (exito ? "exitosa" : "fallida") << endl;
    mostrarEstadoJugador(j1);
    
    cout << "\n--- Retirar dinero (sin fondos) ---" << endl;
    exito = retirarDinero(j2, 2000);
    cout << "Operación " << (exito ? "exitosa" : "fallida") << endl;
    mostrarEstadoJugador(j2);
    
    // ===== COMPRA DE PROPIEDADES =====
    imprimirSeparador();
    cout << "4. PRUEBAS DE COMPRA DE PROPIEDADES" << endl;
    
    cout << "\n--- Comprar propiedades ---" << endl;
    comprarPropiedad(j1, "Boardwalk", 400);
    comprarPropiedad(j1, "Park Place", 350);
    comprarPropiedad(j1, "Pennsylvania Avenue", 320);
    mostrarEstadoJugador(j1);
    
    cout << "\n--- Intentar comprar sin dinero ---" << endl;
    comprarPropiedad(j2, "Boardwalk", 400);
    mostrarEstadoJugador(j2);
    
    // ===== TRANSFERENCIAS ENTRE JUGADORES =====
    imprimirSeparador();
    cout << "5. PRUEBAS DE TRANSFERENCIAS" << endl;
    
    cout << "\n--- Pagar a otro jugador ---" << endl;
    pagarA(j1, j2, 200);
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    
    cout << "\n--- Cobrar alquiler ---" << endl;
    j2.posicion = 37;  // j2 cae en propiedad de j1
    cobrarAlquiler(j2, j1, 150);
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    
    // ===== OPERACIONES CON EL BANCO =====
    imprimirSeparador();
    cout << "6. PRUEBAS CON EL BANCO" << endl;
    
    cout << "\n--- Pagar impuesto ---" << endl;
    pagarImpuesto(j1, 200, "Lujo");
    mostrarEstadoJugador(j1);
    
    cout << "\n--- Cobrar premio ---" << endl;
    cobrarPremio(j2, 150, "Has ganado el segundo premio de belleza");
    mostrarEstadoJugador(j2);
    
    // ===== OPERACIONES DE CÁRCEL =====
    imprimirSeparador();
    cout << "7. PRUEBAS DE CÁRCEL" << endl;
    
    cout << "\n--- Pagar multa para salir de cárcel ---" << endl;
    mostrarEstadoJugador(j3);
    pagarMultaCarcel(j3);
    mostrarEstadoJugador(j3);
    
    // ===== OPERACIONES CON MÚLTIPLES JUGADORES =====
    imprimirSeparador();
    cout << "8. PRUEBAS CON MÚLTIPLES JUGADORES" << endl;
    
    vector<Jugador*> todosJugadores = {&j1, &j2, &j3};
    
    cout << "\n--- Pagar a todos los jugadores ---" << endl;
    cout << "Estado antes:" << endl;
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    mostrarEstadoJugador(j3);
    
    pagarATodos(j1, todosJugadores, 50);
    
    cout << "\nEstado después:" << endl;
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    mostrarEstadoJugador(j3);
    
    cout << "\n--- Cobrar de todos los jugadores ---" << endl;
    cobrarDeTodos(j2, todosJugadores, 25);
    
    cout << "\nEstado después:" << endl;
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    mostrarEstadoJugador(j3);
    
    // ===== PRUEBA DE QUIEBRA =====
    imprimirSeparador();
    cout << "9. PRUEBA DE QUIEBRA" << endl;
    
    cout << "\n--- Forzar situación de quiebra ---" << endl;
    j3.dinero = 100;  // Reducir dinero de j3
    mostrarEstadoJugador(j3);
    
    cout << "\nj3 intenta pagar $500 a j1 pero no puede..." << endl;
    bool pudoPagar = pagarA(j3, j1, 500);
    
    if (!pudoPagar)
    {
        cout << "\n¡j3 debe declararse en quiebra!" << endl;
        transferirTodoA(j3, j1);
    }
    
    cout << "\nEstado final:" << endl;
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j3);
    
    // ===== RESUMEN FINAL =====
    imprimirSeparador();
    cout << "10. RESUMEN FINAL DE TODOS LOS JUGADORES" << endl;
    
    mostrarEstadoJugador(j1);
    mostrarEstadoJugador(j2);
    mostrarEstadoJugador(j3);
    
    // Determinar ganador
    cout << "\n--- ESTADÍSTICAS ---" << endl;
    vector<Jugador*> jugadoresActivos;
    
    for (Jugador* j : todosJugadores)
    {
        if (!j->estaQuebrado)
        {
            jugadoresActivos.push_back(j);
        }
    }
    
    cout << "Jugadores activos: " << jugadoresActivos.size() << "/" << todosJugadores.size() << endl;
    
    if (jugadoresActivos.size() > 0)
    {
        Jugador* masRico = jugadoresActivos[0];
        for (Jugador* j : jugadoresActivos)
        {
            if (j->dinero > masRico->dinero)
            {
                masRico = j;
            }
        }
        
        cout << "\nJugador más rico: " << masRico->nombre 
             << " con $" << masRico->dinero << endl;
    }
    
    imprimirSeparador();
    cout << "FIN DE LAS PRUEBAS" << endl;
    cout << "====================================" << endl;
    
    return 0;
}