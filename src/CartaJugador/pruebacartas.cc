#include "cartas.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
	// Vector para guardar todas las cartas de Suerte
	vector<Carta> cartasSuerte;
	// Vector para guardar todas las cartas de Cofre
	vector<Carta> cartasCofre;
	
	// Carta temporal para leer del archivo
	Carta temp;
	
	// Variables temporales para leer del archivo
	string myText;
	ifstream MyReadFile("cartas.txt");
	
	int contador = 0;
	
	// Ciclo para leer cada linea del archivo
	while(getline(MyReadFile, myText))
	{
		if(myText != "===")
		{
			switch(contador)
			{
				case 0: // Primera linea: descripcion de la carta
					temp.descripcion = myText;
					contador++;
					break;
				case 1: // Segunda linea: tipo (SUERTE o COFRE)
					temp.tipo = myText;
					contador++;
					break;
				case 2: // Tercera linea: accion
					temp.accion = myText;
					contador++;
					break;
				case 3: // Cuarta linea: valor
					temp.valor = stoi(myText);
					temp.valorExtra = 0; // Inicializar en 0 por defecto
					contador++;
					break;
				case 4: // Quinta linea (opcional): valor extra para reparaciones
					temp.valorExtra = stoi(myText);
					contador++;
					break;
			}
		}
		else // Separador encontrado, guardar la carta
		{
			// Agregar la carta al vector correspondiente
			if(temp.tipo == "SUERTE")
				cartasSuerte.push_back(temp);
			else if(temp.tipo == "COFRE")
				cartasCofre.push_back(temp);
			
			// Reiniciar contador
			contador = 0;
		}
	}
	
	// Cerrar archivo
	MyReadFile.close();
	
	// Inicializar generador de numeros aleatorios
	srand(time(0));
	
	// Menu de prueba
	int opcion;
	bool salir = false;
	
	// Variables simuladas del jugador
	int dineroJugador = 1500;
	int posicionJugador = 0;
	bool tieneCartaSalirCarcel = false;
	int numCasas = 0;
	int numHoteles = 0;
	
	cout << "========================================" << endl;
	cout << "   SISTEMA DE CARTAS DE MONOPOLY" << endl;
	cout << "========================================" << endl;
	cout << "Cartas de Suerte cargadas: " << cartasSuerte.size() << endl;
	cout << "Cartas de Cofre cargadas: " << cartasCofre.size() << endl;
	cout << endl;
	
	while(!salir)
	{
		cout << "\n--- ESTADO DEL JUGADOR ---" << endl;
		cout << "Dinero: $" << dineroJugador << endl;
		cout << "Posicion: Casilla " << posicionJugador << endl;
		cout << "Casas: " << numCasas << " | Hoteles: " << numHoteles << endl;
		cout << "Carta Salir Carcel: " << (tieneCartaSalirCarcel ? "SI" : "NO") << endl;
		cout << endl;
		
		cout << "========================================" << endl;
		cout << "1. Sacar carta de SUERTE" << endl;
		cout << "2. Sacar carta de COFRE" << endl;
		cout << "3. Ver todas las cartas de SUERTE" << endl;
		cout << "4. Ver todas las cartas de COFRE" << endl;
		cout << "5. Modificar propiedades del jugador" << endl;
		cout << "0. Salir" << endl;
		cout << "========================================" << endl;
		cout << "Opcion: ";
		cin >> opcion;
		
		switch(opcion)
		{
			case 1: // Sacar carta de Suerte
			{
				if(cartasSuerte.size() > 0)
				{
					int indice = rand() % cartasSuerte.size();
					Carta cartaAleatoria = cartasSuerte[indice];
					ejecutarCarta(cartaAleatoria, dineroJugador, posicionJugador, 
								  tieneCartaSalirCarcel, numCasas, numHoteles);
				}
				else
				{
					cout << "No hay cartas de Suerte disponibles" << endl;
				}
				break;
			}
			case 2: // Sacar carta de Cofre
			{
				if(cartasCofre.size() > 0)
				{
					int indice = rand() % cartasCofre.size();
					Carta cartaAleatoria = cartasCofre[indice];
					ejecutarCarta(cartaAleatoria, dineroJugador, posicionJugador, 
								  tieneCartaSalirCarcel, numCasas, numHoteles);
				}
				else
				{
					cout << "No hay cartas de Cofre disponibles" << endl;
				}
				break;
			}
			case 3: // Ver todas las cartas de Suerte
			{
				cout << "\n=== CARTAS DE SUERTE ===" << endl;
				for(int i = 0; i < cartasSuerte.size(); i++)
				{
					cout << "\nCarta " << (i+1) << ":" << endl;
					mostrarCarta(cartasSuerte[i]);
				}
				break;
			}
			case 4: // Ver todas las cartas de Cofre
			{
				cout << "\n=== CARTAS DE COFRE ===" << endl;
				for(int i = 0; i < cartasCofre.size(); i++)
				{
					cout << "\nCarta " << (i+1) << ":" << endl;
					mostrarCarta(cartasCofre[i]);
				}
				break;
			}
			case 5: // Modificar propiedades
			{
				cout << "\nModificar estado del jugador:" << endl;
				cout << "Casas actuales: " << numCasas << " | Nuevas: ";
				cin >> numCasas;
				cout << "Hoteles actuales: " << numHoteles << " | Nuevos: ";
				cin >> numHoteles;
				cout << "Dinero actual: $" << dineroJugador << " | Nuevo: $";
				cin >> dineroJugador;
				break;
			}
			case 0:
				salir = true;
				cout << "Gracias por jugar!" << endl;
				break;
			default:
				cout << "Opcion invalida" << endl;
				break;
		}
	}
	
	return 0;
}