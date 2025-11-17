// ===================================================================
// IMPLEMENTACION DEL TAD CARTAS
// ===================================================================
#ifndef __CARTAS_H
#define __CARTAS_H

// **********************************
// Librerias necesarias
// **********************************
#include <string>
#include <iostream>

// **********************************
// Estructuras de datos
// **********************************
struct cart
{
	std::string descripcion;
	std::string tipo; // SUERTE o COFRE
	std::string accion; // COBRAR, PAGAR, MOVER, SALIR_CARCEL, IR_CARCEL, REPARACIONES
	int valor; // Cantidad de dinero a cobrar/pagar o casilla a la que moverse
	int valorExtra; // Para reparaciones: valor por casa y por hotel
};
typedef struct cart Carta;

// **********************************
// Operaciones
// **********************************

Carta crearCarta(std::string descripcion, std::string tipo, std::string accion, 
	int valor, int valorExtra = 0)
{
	Carta c;
	
	c.descripcion = descripcion;
	c.tipo = tipo;
	c.accion = accion;
	c.valor = valor;
	c.valorExtra = valorExtra;
	
	return c;
}

void mostrarCarta(Carta c)
{
	std::cout << "========================================" << std::endl;
	std::cout << "  " << c.tipo << std::endl;
	std::cout << "========================================" << std::endl;
	std::cout << c.descripcion << std::endl;
	std::cout << "========================================" << std::endl;
	
	// Información de debug (opcional, comentar en producción)
	std::cout << "[Acción: " << c.accion << ", Valor: " << c.valor;
	if(c.valorExtra != 0)
		std::cout << ", Valor Extra: " << c.valorExtra;
	std::cout << "]" << std::endl;
}

std::string obtenerDescripcion(Carta c)
{
	return c.descripcion;
}

std::string obtenerTipo(Carta c)
{
	return c.tipo;
}

std::string obtenerAccion(Carta c)
{
	return c.accion;
}

int obtenerValor(Carta c)
{
	return c.valor;
}

int obtenerValorExtra(Carta c)
{
	return c.valorExtra;
}

void ejecutarCarta(Carta c, int& dineroJugador, int& posicionJugador, 
	bool& tieneCartaSalirCarcel, int numCasas, int numHoteles)
{
	std::cout << std::endl;
	mostrarCarta(c);
	std::cout << std::endl;
	
	if(c.accion == "COBRAR")
	{
		dineroJugador += c.valor;
		std::cout << "Has recibido $" << c.valor << std::endl;
	}
	else if(c.accion == "PAGAR")
	{
		dineroJugador -= c.valor;
		std::cout << "Debes pagar $" << c.valor << std::endl;
	}
	else if(c.accion == "MOVER")
	{
		posicionJugador = c.valor;
		std::cout << "Te mueves a la casilla " << c.valor << std::endl;
	}
	else if(c.accion == "RETROCEDER")
	{
		posicionJugador -= c.valor;
		if(posicionJugador < 0)
			posicionJugador += 40; // El tablero tiene 40 casillas
		std::cout << "Retrocedes " << c.valor << " casillas" << std::endl;
	}
	else if(c.accion == "AVANZAR")
	{
		posicionJugador += c.valor;
		if(posicionJugador >= 40)
			posicionJugador -= 40;
		std::cout << "Avanzas " << c.valor << " casillas" << std::endl;
	}
	else if(c.accion == "SALIR_CARCEL")
	{
		tieneCartaSalirCarcel = true;
		std::cout << "Guardas esta carta para salir de la cárcel" << std::endl;
	}
	else if(c.accion == "IR_CARCEL")
	{
		posicionJugador = 10; // Casilla de la cárcel
		std::cout << "Vas directamente a la cárcel" << std::endl;
	}
	else if(c.accion == "REPARACIONES")
	{
		int costoTotal = (numCasas * c.valor) + (numHoteles * c.valorExtra);
		dineroJugador -= costoTotal;
		std::cout << "Pagas $" << c.valor << " por cada casa y $" 
				  << c.valorExtra << " por cada hotel" << std::endl;
		std::cout << "Total a pagar: $" << costoTotal << std::endl;
	}
	else if(c.accion == "COBRAR_JUGADORES")
	{
		// Esta acción requiere implementación externa para cobrar a otros jugadores
		std::cout << "Cada jugador debe pagarte $" << c.valor << std::endl;
	}
	else if(c.accion == "PAGAR_JUGADORES")
	{
		// Esta acción requiere implementación externa para pagar a otros jugadores
		std::cout << "Debes pagar $" << c.valor << " a cada jugador" << std::endl;
	}
	
	std::cout << "Dinero actual: $" << dineroJugador << std::endl;
}

#endif