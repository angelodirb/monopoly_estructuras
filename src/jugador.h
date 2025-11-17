// ===================================================================
// IMPLEMENTACION DEL TAD JUGADOR
// ===================================================================
#ifndef __JUGADOR_H
#define __JUGADOR_H

// **********************************
// Librerias necesarias
// **********************************
#include <cstring>
#include <iostream>

// **********************************
// Estructuras de datos
// **********************************
struct player
{
	std::string nombre;                     // Nombre del objeto del jugador ejemplo: SOMBRERO, BARCO...
	int dinero;                             // Saldo del jugador
	int posicion;                           // posicion en casilla del jugador en el tablero
	std::vector<std::string> propiedades;   // Vector que contiene el nombre de las propiedades del jugador
    int turnosCarcel;                        // turnos que el jugador ha pasado en la carcel (si llega a 3 y no ha salido de la carcel automaticamente debe pagar 50 para salir)
    bool enCarcel;                          // Verdadero si esta en la carcel, falso si no
	bool estaQuebrado;                      // Si el jugador queda en bancarrota pierde
    bool tieneCarta;                        // permite verificar si el usuario posee la carta para salir de la carcel gratis
};
typedef struct player Jugador;

// **********************************
// Operaciones
// **********************************

Jugador crearJugador(std::string nombre)
{
	Jugador j;

	j.nombre = nombre;
	j.dinero = 1500;
	j.posicion = 0;
	j.turnosCarcel = 0;
    j.enCarcel = false;
    j.estaQuebrado = false;
    j.tieneCarta = false;
	return j;
}

// **********************************
// Operaciones de Movimiento
// **********************************

// Constantes del tablero !POR AHORA SE DEFINEN ASI!
const int CASILLAS_TOTALES = 40;
const int CASILLA_CARCEL = 10;
const int CASILLA_SALIDA = 0;
const int DINERO_POR_SALIDA = 200;

/**
 * Mueve al jugador una cantidad de casillas desde su posición actual
 * Si pasa por la salida (casilla 0), recibe $200
 */
void moverJugador(Jugador& j, int casillas)
{
    // Guardar posición anterior para verificar si pasó por salida
    int posicionAnterior = j.posicion;
    
    // Calcular nueva posición (con wraparound del tablero)
    j.posicion = (j.posicion + casillas) % CASILLAS_TOTALES;
    
    // Verificar si pasó por la salida
    // Esto ocurre cuando la suma excede CASILLAS_TOTALES
    if (posicionAnterior + casillas >= CASILLAS_TOTALES && casillas > 0)
    {
        j.dinero += DINERO_POR_SALIDA;
        std::cout << j.nombre << " pasó por la SALIDA y recibe $" << DINERO_POR_SALIDA << std::endl;
    }
    
    std::cout << j.nombre << " se movió " << casillas << " casillas a la posición " 
              << j.posicion << std::endl;
}

/**
 * Mueve al jugador directamente a una casilla específica
 * NO cobra por pasar por salida (usado para cartas especiales)
 */
void irACasilla(Jugador& j, int casilla)
{
    // Validar que la casilla esté en rango
    if (casilla < 0 || casilla >= CASILLAS_TOTALES)
    {
        std::cerr << "Error: Casilla fuera de rango" << std::endl;
        return;
    }
    
    j.posicion = casilla;
    std::cout << j.nombre << " fue enviado directamente a la casilla " 
              << casilla << std::endl;
}

/**
 * Variante que SÍ cobra por pasar por salida si la casilla está "adelante"
 * Usado para cartas como "Avanza hasta..."
 */
void irACasillaConSalida(Jugador& j, int casilla)
{
    // Validar que la casilla esté en rango
    if (casilla < 0 || casilla >= CASILLAS_TOTALES)
    {
        std::cerr << "Error: Casilla fuera de rango" << std::endl;
        return;
    }
    
    // Si la casilla está "atrás" en el tablero, significa que pasará por salida
    if (casilla <= j.posicion)
    {
        j.dinero += DINERO_POR_SALIDA;
        std::cout << j.nombre << " pasó por la SALIDA y recibe $" << DINERO_POR_SALIDA << std::endl;
    }
    
    j.posicion = casilla;
    std::cout << j.nombre << " avanzó hasta la casilla " << casilla << std::endl;
}

/**
 * Envía al jugador a la cárcel
 * Lo coloca en la casilla de cárcel y activa el estado de encarcelado
 */
void enviarACarcel(Jugador& j)
{
    j.posicion = CASILLA_CARCEL;
    j.enCarcel = true;
    j.turnosCarcel = 0;
    
    std::cout << "¡" << j.nombre << " va a la CÁRCEL!" << std::endl;
}

/**
 * Retrocede al jugador una cantidad de casillas
 * Útil para algunas cartas especiales
 */
void retrocederJugador(Jugador& j, int casillas)
{
    j.posicion = (j.posicion - casillas + CASILLAS_TOTALES) % CASILLAS_TOTALES;
    
    std::cout << j.nombre << " retrocedió " << casillas 
              << " casillas a la posición " << j.posicion << std::endl;
}

/**
 * Mueve al jugador a la propiedad más cercana de un tipo
 * Ejemplo: "Avanza hasta el ferrocarril más cercano"
 */
void irAPropiedadMasCercana(Jugador& j, std::vector<int> casillasPropiedades)
{
    int distanciaMinima = CASILLAS_TOTALES;
    int casillaDestino = j.posicion;
    
    // Buscar la propiedad más cercana "hacia adelante"
    for (int casilla : casillasPropiedades)
    {
        int distancia;
        if (casilla >= j.posicion)
        {
            distancia = casilla - j.posicion;
        }
        else
        {
            distancia = (CASILLAS_TOTALES - j.posicion) + casilla;
        }
        
        if (distancia < distanciaMinima)
        {
            distanciaMinima = distancia;
            casillaDestino = casilla;
        }
    }
    
    // Usar irACasillaConSalida para que cobre si pasa por salida
    irACasillaConSalida(j, casillaDestino);
}

/**
 * Ejemplo de uso con ferrocarriles (casillas 5, 15, 25, 35 en Monopoly estándar)
 */
void irAFerrocarrilMasCercano(Jugador& j)
{
    std::vector<int> ferrocarriles = {5, 15, 25, 35};
    irAPropiedadMasCercana(j, ferrocarriles);
}

/**
 * Ejemplo de uso con servicios públicos (casillas 12, 28 en Monopoly estándar)
 */
void irAServicioMasCercano(Jugador& j)
{
    std::vector<int> servicios = {12, 28};
    irAPropiedadMasCercana(j, servicios);
}

// **********************************
// Operaciones Financieras
// **********************************

/**
 * Agrega dinero al jugador
 * Usado para premios, cobrar alquiler, pasar por salida, etc.
 */
void agregarDinero(Jugador& j, int cantidad)
{
    if (cantidad < 0)
    {
        std::cerr << "Error: No se puede agregar cantidad negativa" << std::endl;
        return;
    }
    
    j.dinero += cantidad;
    std::cout << j.nombre << " recibe $" << cantidad 
              << " (Saldo: $" << j.dinero << ")" << std::endl;
}

/**
 * Retira dinero del jugador
 * Retorna true si la operación fue exitosa
 * Retorna false si no tiene suficiente dinero
 */
bool retirarDinero(Jugador& j, int cantidad)
{
    if (cantidad < 0)
    {
        std::cerr << "Error: No se puede retirar cantidad negativa" << std::endl;
        return false;
    }
    
    if (j.dinero < cantidad)
    {
        std::cout << j.nombre << " no tiene suficiente dinero. "
                  << "Necesita $" << cantidad << " pero solo tiene $" << j.dinero << std::endl;
        return false;
    }
    
    j.dinero -= cantidad;
    std::cout << j.nombre << " paga $" << cantidad 
              << " (Saldo: $" << j.dinero << ")" << std::endl;
    
    return true;
}

/**
 * Transfiere dinero de un jugador a otro
 * Retorna true si la operación fue exitosa
 * Retorna false si el pagador no tiene suficiente dinero
 */
bool pagarA(Jugador& pagador, Jugador& receptor, int cantidad)
{
    if (cantidad < 0)
    {
        std::cerr << "Error: No se puede pagar cantidad negativa" << std::endl;
        return false;
    }
    
    if (pagador.dinero < cantidad)
    {
        std::cout << pagador.nombre << " no puede pagar $" << cantidad 
                  << " a " << receptor.nombre << std::endl;
        return false;
    }
    
    pagador.dinero -= cantidad;
    receptor.dinero += cantidad;
    
    std::cout << pagador.nombre << " paga $" << cantidad << " a " 
              << receptor.nombre << std::endl;
    std::cout << "  - " << pagador.nombre << ": $" << pagador.dinero << std::endl;
    std::cout << "  - " << receptor.nombre << ": $" << receptor.dinero << std::endl;
    
    return true;
}

/**
 * Paga al banco (dinero desaparece del juego)
 * Retorna true si pudo pagar, false si no tiene suficiente
 */
bool pagarAlBanco(Jugador& j, int cantidad)
{
    if (cantidad < 0)
    {
        std::cerr << "Error: No se puede pagar cantidad negativa" << std::endl;
        return false;
    }
    
    if (j.dinero < cantidad)
    {
        std::cout << j.nombre << " no puede pagar $" << cantidad 
                  << " al Banco" << std::endl;
        return false;
    }
    
    j.dinero -= cantidad;
    std::cout << j.nombre << " paga $" << cantidad << " al Banco "
              << "(Saldo: $" << j.dinero << ")" << std::endl;
    
    return true;
}

/**
 * Cobra del banco (dinero aparece en el juego)
 */
void cobrarDelBanco(Jugador& j, int cantidad)
{
    if (cantidad < 0)
    {
        std::cerr << "Error: No se puede cobrar cantidad negativa" << std::endl;
        return;
    }
    
    j.dinero += cantidad;
    std::cout << j.nombre << " cobra $" << cantidad << " del Banco "
              << "(Saldo: $" << j.dinero << ")" << std::endl;
}

/**
 * Declara la quiebra del jugador
 * El jugador pierde todo su dinero y propiedades
 */
void declararQuiebra(Jugador& j)
{
    j.estaQuebrado = true;
    j.dinero = 0;
    j.propiedades.clear();
    
    std::cout << "¡" << j.nombre << " está EN QUIEBRA! Ha perdido el juego." << std::endl;
}

/**
 * Transfiere todas las propiedades y dinero de un jugador a otro
 * Usado cuando un jugador debe todo a otro y no puede pagar
 */
void transferirTodoA(Jugador& quebrado, Jugador& acreedor)
{
    // Transferir dinero
    acreedor.dinero += quebrado.dinero;
    quebrado.dinero = 0;
    
    // Transferir propiedades
    for (const std::string& propiedad : quebrado.propiedades)
    {
        acreedor.propiedades.push_back(propiedad);
    }
    
    std::cout << quebrado.nombre << " transfiere todo a " << acreedor.nombre << ":" << std::endl;
    std::cout << "  - Propiedades transferidas: " << quebrado.propiedades.size() << std::endl;
    std::cout << "  - " << acreedor.nombre << " ahora tiene $" << acreedor.dinero << std::endl;
    
    // Declarar quiebra
    declararQuiebra(quebrado);
}

/**
 * Verifica si el jugador puede pagar una cantidad
 * No modifica el estado del jugador
 */
bool puedeComprar(const Jugador& j, int precio)
{
    return j.dinero >= precio;
}

/**
 * Calcula el patrimonio total del jugador
 * (dinero en efectivo + valor de propiedades)
 * Nota: Necesitarás pasar información adicional sobre el valor de las propiedades
 */
int calcularPatrimonio(const Jugador& j, int valorPorPropiedad = 0)
{
    int patrimonio = j.dinero;
    patrimonio += j.propiedades.size() * valorPorPropiedad;
    
    return patrimonio;
}

/**
 * Intenta comprar una propiedad
 * Retorna true si la compra fue exitosa
 */
bool comprarPropiedad(Jugador& j, std::string nombrePropiedad, int precio)
{
    if (!puedeComprar(j, precio))
    {
        std::cout << j.nombre << " no puede comprar " << nombrePropiedad 
                  << " (Cuesta $" << precio << ", tiene $" << j.dinero << ")" << std::endl;
        return false;
    }
    
    j.dinero -= precio;
    j.propiedades.push_back(nombrePropiedad);
    
    std::cout << j.nombre << " compra " << nombrePropiedad << " por $" << precio 
              << " (Saldo: $" << j.dinero << ")" << std::endl;
    
    return true;
}

/**
 * Cobra alquiler de una propiedad
 * El dueño cobra al inquilino
 */
bool cobrarAlquiler(Jugador& inquilino, Jugador& dueno, int alquiler)
{
    if (inquilino.dinero < alquiler)
    {
        std::cout << inquilino.nombre << " no puede pagar alquiler de $" << alquiler 
                  << " a " << dueno.nombre << std::endl;
        std::cout << "¡" << inquilino.nombre << " debe declararse en quiebra!" << std::endl;
        return false;
    }
    
    return pagarA(inquilino, dueno, alquiler);
}

/**
 * Paga impuesto (lujo o renta)
 * Retorna true si pudo pagar
 */
bool pagarImpuesto(Jugador& j, int cantidad, std::string tipoImpuesto = "")
{
    if (tipoImpuesto != "")
    {
        std::cout << j.nombre << " debe pagar impuesto de " << tipoImpuesto << std::endl;
    }
    
    return pagarAlBanco(j, cantidad);
}

/**
 * Paga multa de cárcel ($50 en Monopoly clásico)
 */
bool pagarMultaCarcel(Jugador& j)
{
    const int MULTA_CARCEL = 50;
    
    if (!pagarAlBanco(j, MULTA_CARCEL))
    {
        return false;
    }
    
    // Si pagó exitosamente, sale de la cárcel
    j.enCarcel = false;
    j.turnosCarcel = 0;
    std::cout << j.nombre << " pagó la multa y sale de la CÁRCEL" << std::endl;
    
    return true;
}

/**
 * Cobra premio (por ejemplo, del centro de la banca)
 */
void cobrarPremio(Jugador& j, int cantidad, std::string descripcion = "")
{
    if (descripcion != "")
    {
        std::cout << j.nombre << ": " << descripcion << std::endl;
    }
    
    cobrarDelBanco(j, cantidad);
}

/**
 * Paga a todos los jugadores (por ejemplo, carta de "Paga $50 a cada jugador")
 */
bool pagarATodos(Jugador& pagador, std::vector<Jugador*>& jugadores, int cantidad)
{
    int totalAPagar = cantidad * (jugadores.size() - 1); // -1 para no contarse a sí mismo
    
    if (pagador.dinero < totalAPagar)
    {
        std::cout << pagador.nombre << " no puede pagar $" << cantidad 
                  << " a cada jugador (necesita $" << totalAPagar << ")" << std::endl;
        return false;
    }
    
    for (Jugador* j : jugadores)
    {
        if (j->nombre != pagador.nombre) // No pagarse a sí mismo
        {
            pagarA(pagador, *j, cantidad);
        }
    }
    
    return true;
}

/**
 * Cobra de todos los jugadores (por ejemplo, "Cada jugador te paga $10")
 */
void cobrarDeTodos(Jugador& receptor, std::vector<Jugador*>& jugadores, int cantidad)
{
    std::cout << receptor.nombre << " cobra $" << cantidad << " de cada jugador:" << std::endl;
    
    for (Jugador* j : jugadores)
    {
        if (j->nombre != receptor.nombre) // No cobrarse a sí mismo
        {
            if (j->dinero >= cantidad)
            {
                pagarA(*j, receptor, cantidad);
            }
            else
            {
                std::cout << "  - " << j->nombre << " no tiene suficiente dinero" << std::endl;
            }
        }
    }
}