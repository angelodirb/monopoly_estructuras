#ifndef JUEGO_H_
#define JUEGO_H_

#include "tablero.h"
#include "Banco.h"
#include "Dado.h"
#include "cola.h"
#include "jugador.h"
#include "cartas.h"
#include "ColaCartas.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// **********************************
// TAD JUEGO - Sistema principal
// **********************************
class Juego {
private:
    // Estructuras del juego
    ListaCircular tablero;
    Banco banco;
    SistemaCartas sistemaCartas;
    
    
    // Estado del juego
    vector<Jugador> jugadores;
    int turnoActual;
    int rondaActual;
    bool juegoActivo;
    Dado dado;
    
    
    // Control de cárcel
    int turnosEnCarcelActual;
    
    // ===== FUNCIONES PRIVADAS =====
    
    /**
     * Precondición: Tablero cargado
     * Postcondición: Propiedades registradas (usa tabla hash interna del tablero)
     */
    void registrarPropiedades() {
        cout << "Propiedades registradas automáticamente en tabla hash" << endl;
    }
    
    /**
     * Precondición: Archivo de cartas debe existir
     * Postcondición: Cartas cargadas en colas
     */
    void cargarCartas() {
        cout << "Cargando sistema de cartas..." << endl;
        
        if (sistemaCartas.inicializar("src/cartas.txt")) {
            cout << "Sistema de cartas cargado exitosamente" << endl;
            sistemaCartas.mostrarEstado();
        } else {
            cout << "Advertencia: Sistema de cartas no disponible" << endl;
        }
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Jugadores configurados con dinero inicial
     */
    void configurarJugadores() {
        int numJugadores;
        
        do {
            cout << "\n¿Cuántos jugadores? (2-6): ";
            cin >> numJugadores;
            
            if (numJugadores < 2 || numJugadores > 6) {
                cout << "Número inválido. Debe ser entre 2 y 6." << endl;
            }
        } while (numJugadores < 2 || numJugadores > 6);
        
        string nombres[] = {"SOMBRERO", "PERRO", "BARCO", "ZAPATO", "CARRO", "DEDAL"};
        
        cout << "\n👤 Configurando jugadores:" << endl;
        for (int i = 0; i < numJugadores; i++) {
            Jugador j = crearJugador(nombres[i]);
            jugadores.push_back(j);
        }
        
        // El banco distribuye dinero inicial
        bancoDistribuirDineroInicial(banco, jugadores, 1500);
    }
    
    /**
     * Precondición: Jugador válido
     * Postcondición: Retorna la casilla actual del jugador
     */
    Casilla* obtenerCasillaActual(const Jugador& jugador) {
        Casilla* actual = getCabeza(tablero);
        
        for (int i = 0; i < jugador.posicion; i++) {
            actual = actual->siguiente;
        }
        
        return actual;
    }
    
    /**
     * Precondición: Jugador válido, cantidad > 0
     * Postcondición: Procesa el movimiento y la casilla donde cae
     */
    void procesarMovimiento(Jugador& jugador, int casillas) {
        int posicionAnterior = jugador.posicion;
        
        // Calcular nueva posición
        jugador.posicion = (jugador.posicion + casillas) % 40;
        
        cout << "\n " << jugador.nombre << " se mueve " << casillas 
             << " casillas (de " << posicionAnterior << " a " << jugador.posicion << ")" << endl;
        
        // Verificar si pasó por SALIDA
        if (posicionAnterior > jugador.posicion || 
            (posicionAnterior + casillas >= 40)) {
            cout << " " << jugador.nombre << " pasó por SALIDA - Recibe $200" << endl;
            bancoPagarSalida(banco, jugador);
        }
    }
    
    /**
     * Precondición: Jugador válido
     * Postcondición: Procesa la acción de la casilla donde cae
     */
    void procesarCasilla(Jugador& jugador) {
        Casilla* casilla = obtenerCasillaActual(jugador);
        
        if (casilla == nullptr) {
            cout << "Error: Casilla nula" << endl;
            return;
        }
        
        string nombreCasilla = casilla->getNombre();
        
        cout << "\n " << jugador.nombre << " cae en: " << nombreCasilla << endl;
        
        // ===== CASILLAS ESPECIALES =====
        
        if (nombreCasilla == "SALIDA") {
            cout << " Estás en SALIDA" << endl;
        }
        else if (nombreCasilla == "CARCEL") {
            if (!jugador.enCarcel) {
                cout << "  Estás visitando la cárcel (sin estar preso)" << endl;
            }
        }
        else if (nombreCasilla == "PARQUEADERO GRATUITO") {
            cout << " Parqueadero gratuito - Descansa sin pagar" << endl;
        }
        else if (nombreCasilla == "IR A LA CARCEL") {
            cout << " ¡IR A LA CÁRCEL! No pases por SALIDA" << endl;
            enviarACarcel(jugador);
        }
        else if (nombreCasilla.find("SUERTE") != string::npos || 
                 nombreCasilla.find("CHANCE") != string::npos) {
            procesarCartaSuerte(jugador);
        }
        else if (nombreCasilla.find("COMUNIDAD") != string::npos || 
                 nombreCasilla.find("COFRE") != string::npos) {
            procesarCartaCofre(jugador);
        }
        
        // ===== PROPIEDADES Y SERVICIOS =====
        else {
            procesarPropiedad(jugador, casilla);
        }
    }
    
    /**
     * Precondición: Jugador válido, casilla es una propiedad
     * Postcondición: Procesa compra o pago de alquiler
     */
    void procesarPropiedad(Jugador& jugador, Casilla* casilla) {
        Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
        Ferrocarril* ferro = dynamic_cast<Ferrocarril*>(casilla);
        Servicio* serv = dynamic_cast<Servicio*>(casilla);

        // ===== PROPIEDAD NORMAL =====
        if (prop != nullptr) {
            cout << "🏠 PROPIEDAD: " << casilla->getNombre() << endl;
            prop->mostrarInfo();

            string duenio = prop->getDuenio();

            // Sin dueño - Opción de compra
            if (duenio.empty()) {
                cout << "\n💰 Esta propiedad está disponible por $" << prop->getPrecio() << endl;
                cout << "¿Deseas comprarla? (s/n): ";
                char respuesta;
                cin >> respuesta;

                if (respuesta == 's' || respuesta == 'S') {
                    if (bancoCobrarDinero(banco, jugador, prop->getPrecio(), "Compra de " + casilla->getNombre())) {
                        prop->setDuenio(jugador.nombre);
                        jugador.propiedades.push_back(casilla->getNombre());
                        cout << "✅ " << jugador.nombre << " compró " << casilla->getNombre() << endl;
                    } else {
                        cout << "❌ No tienes suficiente dinero para comprar esta propiedad" << endl;
                    }
                } else {
                    cout << "❌ " << jugador.nombre << " decidió no comprar" << endl;
                }
            }
            // Tiene dueño - Cobrar alquiler
            else if (duenio != jugador.nombre) {
                int alquiler = prop->obtenerAlquiler();
                cout << "\n💸 Esta propiedad pertenece a " << duenio << endl;
                cout << "💰 Debes pagar alquiler: $" << alquiler << endl;

                // Buscar al dueño en la lista de jugadores
                Jugador* duenioPtr = nullptr;
                for (Jugador& j : jugadores) {
                    if (j.nombre == duenio) {
                        duenioPtr = &j;
                        break;
                    }
                }

                if (duenioPtr != nullptr) {
                    if (!bancoTransferencia(banco, jugador, *duenioPtr, alquiler, "Alquiler de " + casilla->getNombre())) {
                        cout << "⚠️ No tienes suficiente dinero para pagar el alquiler" << endl;
                    }
                } else {
                    // Si no se encuentra el dueño, solo cobrar al banco
                    if (!bancoCobrarDinero(banco, jugador, alquiler, "Alquiler de " + casilla->getNombre())) {
                        cout << "⚠️ No tienes suficiente dinero para pagar el alquiler" << endl;
                    }
                }
            }
            // Es del jugador actual
            else {
                cout << "\n✅ Esta es TU propiedad" << endl;
            }
        }

        // ===== FERROCARRIL =====
        else if (ferro != nullptr) {
            cout << "🚂 FERROCARRIL: " << casilla->getNombre() << endl;
            ferro->activar();

            string duenio = ferro->getDuenio();

            // Sin dueño - Opción de compra
            if (duenio.empty()) {
                cout << "\n💰 Este ferrocarril está disponible por $" << ferro->getPrecio() << endl;
                cout << "¿Deseas comprarlo? (s/n): ";
                char respuesta;
                cin >> respuesta;

                if (respuesta == 's' || respuesta == 'S') {
                    if (bancoCobrarDinero(banco, jugador, ferro->getPrecio(), "Compra de " + casilla->getNombre())) {
                        ferro->setDuenio(jugador.nombre);
                        jugador.propiedades.push_back(casilla->getNombre());
                        cout << "✅ " << jugador.nombre << " compró " << casilla->getNombre() << endl;
                    } else {
                        cout << "❌ No tienes suficiente dinero para comprar este ferrocarril" << endl;
                    }
                } else {
                    cout << "❌ " << jugador.nombre << " decidió no comprar" << endl;
                }
            }
            // Tiene dueño - Cobrar alquiler (básico por ahora)
            else if (duenio != jugador.nombre) {
                int alquiler = 25;  // Alquiler base de ferrocarril
                cout << "\n💸 Este ferrocarril pertenece a " << duenio << endl;
                cout << "💰 Debes pagar alquiler: $" << alquiler << endl;

                // Buscar al dueño
                Jugador* duenioPtr = nullptr;
                for (Jugador& j : jugadores) {
                    if (j.nombre == duenio) {
                        duenioPtr = &j;
                        break;
                    }
                }

                if (duenioPtr != nullptr) {
                    if (!bancoTransferencia(banco, jugador, *duenioPtr, alquiler, "Alquiler de " + casilla->getNombre())) {
                        cout << "⚠️ No tienes suficiente dinero para pagar el alquiler" << endl;
                    }
                } else {
                    if (!bancoCobrarDinero(banco, jugador, alquiler, "Alquiler de " + casilla->getNombre())) {
                        cout << "⚠️ No tienes suficiente dinero para pagar el alquiler" << endl;
                    }
                }
            }
            // Es del jugador actual
            else {
                cout << "\n✅ Este es TU ferrocarril" << endl;
            }
        }

        // ===== SERVICIO =====
        else if (serv != nullptr) {
            cout << "⚡ SERVICIO: " << casilla->getNombre() << endl;
            serv->activar();

            string duenio = serv->getDuenio();

            // Sin dueño - Opción de compra
            if (duenio.empty()) {
                cout << "\n💰 Este servicio está disponible por $" << serv->getPrecio() << endl;
                cout << "¿Deseas comprarlo? (s/n): ";
                char respuesta;
                cin >> respuesta;

                if (respuesta == 's' || respuesta == 'S') {
                    if (bancoCobrarDinero(banco, jugador, serv->getPrecio(), "Compra de " + casilla->getNombre())) {
                        serv->setDuenio(jugador.nombre);
                        jugador.propiedades.push_back(casilla->getNombre());
                        cout << "✅ " << jugador.nombre << " compró " << casilla->getNombre() << endl;
                    } else {
                        cout << "❌ No tienes suficiente dinero para comprar este servicio" << endl;
                    }
                } else {
                    cout << "❌ " << jugador.nombre << " decidió no comprar" << endl;
                }
            }
            // Tiene dueño - Cobrar alquiler basado en los dados
            else if (duenio != jugador.nombre) {
                int multiplicador = 4;  // 4x el valor de los dados (simplificado)
                int alquiler = obtenerSuma(dado) * multiplicador;
                cout << "\n💸 Este servicio pertenece a " << duenio << endl;
                cout << "💰 Debes pagar alquiler: $" << alquiler << " (dados × " << multiplicador << ")" << endl;

                // Buscar al dueño
                Jugador* duenioPtr = nullptr;
                for (Jugador& j : jugadores) {
                    if (j.nombre == duenio) {
                        duenioPtr = &j;
                        break;
                    }
                }

                if (duenioPtr != nullptr) {
                    if (!bancoTransferencia(banco, jugador, *duenioPtr, alquiler, "Alquiler de " + casilla->getNombre())) {
                        cout << "⚠️ No tienes suficiente dinero para pagar el alquiler" << endl;
                    }
                } else {
                    if (!bancoCobrarDinero(banco, jugador, alquiler, "Alquiler de " + casilla->getNombre())) {
                        cout << "⚠️ No tienes suficiente dinero para pagar el alquiler" << endl;
                    }
                }
            }
            // Es del jugador actual
            else {
                cout << "\n✅ Este es TU servicio" << endl;
            }
        }
    }

    /**
     * Precondición: color no vacío
     * Postcondición: Retorna el número total de propiedades de ese color en el tablero
     */
    int contarPropiedadesPorColor(const string& color) const {
        if (color.empty()) return 0;

        int total = 0;
        Casilla* actual = getCabeza(tablero);

        do {
            Propiedad* prop = dynamic_cast<Propiedad*>(actual);
            if (prop != nullptr && prop->getColor() == color) {
                total++;
            }
            actual = actual->siguiente;
        } while (actual != getCabeza(tablero));

        return total;
    }

    /**
     * Precondición: jugador válido, color no vacío
     * Postcondición: Retorna true si el jugador tiene monopolio de ese color
     */
    bool tieneMonopolio(const Jugador& jugador, const string& color) const {
        if (color.empty()) return false;

        int totalColor = contarPropiedadesPorColor(color);
        if (totalColor == 0) return false;

        int jugadorTiene = 0;
        for (const string& nombreProp : jugador.propiedades) {
            Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
            if (casilla != nullptr) {
                Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                if (prop != nullptr && prop->getColor() == color) {
                    jugadorTiene++;
                }
            }
        }

        return jugadorTiene == totalColor && totalColor > 0;
    }

    /**
     * Precondición: jugador válido
     * Postcondición: Retorna lista de colores donde el jugador tiene monopolio
     */
    vector<string> obtenerMonopolios(const Jugador& jugador) const {
        vector<string> monopolios;
        vector<string> coloresRevisados;

        for (const string& nombreProp : jugador.propiedades) {
            Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
            if (casilla != nullptr) {
                Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                if (prop != nullptr) {
                    string color = prop->getColor();

                    // Solo revisar cada color una vez
                    if (find(coloresRevisados.begin(), coloresRevisados.end(), color) == coloresRevisados.end()) {
                        coloresRevisados.push_back(color);

                        if (tieneMonopolio(jugador, color)) {
                            monopolios.push_back(color);
                        }
                    }
                }
            }
        }

        return monopolios;
    }

    /**
     * Precondición: jugador válido
     * Postcondición: Permite al jugador construir casas en sus propiedades con monopolio
     */
    void menuConstruccion(Jugador& jugador) {
        vector<string> monopolios = obtenerMonopolios(jugador);

        if (monopolios.empty()) {
            cout << "\n❌ No tienes monopolios completos. Necesitas todas las propiedades de un color para construir." << endl;
            return;
        }

        cout << "\n🏗️ === MENÚ DE CONSTRUCCIÓN ===" << endl;
        cout << "💰 Dinero disponible: $" << jugador.dinero << endl;
        cout << "\n🎨 Monopolios disponibles:" << endl;

        // Mostrar propiedades con monopolio
        vector<Propiedad*> propiedadesConstruibles;
        int contador = 1;

        for (const string& color : monopolios) {
            cout << "\n📍 Color " << color << ":" << endl;

            for (const string& nombreProp : jugador.propiedades) {
                Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
                if (casilla != nullptr) {
                    Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                    if (prop != nullptr && prop->getColor() == color) {
                        cout << "  " << contador << ". " << prop->getNombre()
                             << " | Casas: " << prop->getNumCasas()
                             << " | Costo construcción: $50" << endl;
                        propiedadesConstruibles.push_back(prop);
                        contador++;
                    }
                }
            }
        }

        if (propiedadesConstruibles.empty()) {
            cout << "\n❌ No hay propiedades disponibles para construcción." << endl;
            return;
        }

        cout << "\n0. Volver sin construir" << endl;
        cout << "\n¿En qué propiedad quieres construir? (0-" << propiedadesConstruibles.size() << "): ";

        int opcion;
        cin >> opcion;

        if (opcion == 0) {
            cout << "✅ Volviendo al turno..." << endl;
            return;
        }

        if (opcion < 1 || opcion > (int)propiedadesConstruibles.size()) {
            cout << "❌ Opción inválida" << endl;
            return;
        }

        Propiedad* propSeleccionada = propiedadesConstruibles[opcion - 1];

        // Verificar si ya tiene hotel
        if (propSeleccionada->getNumCasas() >= 5) {
            cout << "❌ Esta propiedad ya tiene un HOTEL (máximo)" << endl;
            return;
        }

        // Costo de construcción
        const int COSTO_CASA = 50;  // Simplificado

        if (jugador.dinero < COSTO_CASA) {
            cout << "❌ No tienes suficiente dinero. Necesitas $" << COSTO_CASA << endl;
            return;
        }

        // Confirmar construcción
        cout << "\n¿Confirmas construir una casa en " << propSeleccionada->getNombre()
             << " por $" << COSTO_CASA << "? (s/n): ";
        char confirmar;
        cin >> confirmar;

        if (confirmar == 's' || confirmar == 'S') {
            if (bancoCobrarDinero(banco, jugador, COSTO_CASA, "Construcción en " + propSeleccionada->getNombre())) {
                propSeleccionada->construirCasa();
                cout << "✅ ¡Construcción exitosa!" << endl;
                cout << "💰 Dinero restante: $" << jugador.dinero << endl;
            } else {
                cout << "❌ Error al procesar el pago" << endl;
            }
        } else {
            cout << "❌ Construcción cancelada" << endl;
        }
    }

    /**
     * Precondición: Jugador válido
     * Postcondición: Procesa completamente una carta de Suerte
     */
    void procesarCartaSuerte(Jugador& jugador) {
        cout << "\n🎴 ¡Sacaste una carta de SUERTE!" << endl;

        Carta carta = sistemaCartas.sacarCartaSuerte();

        // Verificar si la carta es válida
        if (carta.accion.empty()) {
            cout << "⚠️ No hay cartas disponibles en este momento" << endl;
            return;
        }

        mostrarCarta(carta);

        cout << "\nPresiona Enter para continuar...";
        cin.ignore();
        cin.get();

        // Procesar según el tipo de acción
        if (carta.accion == "COBRAR") {
            bancoOtorgarDinero(banco, jugador, carta.valor, carta.descripcion);
        }
        else if (carta.accion == "PAGAR") {
            if (!bancoCobrarDinero(banco, jugador, carta.valor, carta.descripcion)) {
                cout << "⚠️ No tienes suficiente dinero. Debes vender propiedades o declararte en quiebra." << endl;
            }
        }
        else if (carta.accion == "MOVER") {
            int posicionAnterior = jugador.posicion;
            jugador.posicion = carta.valor;
            
            cout << "🚶 Te mueves a la casilla " << carta.valor << endl;
            
            // Verificar si pasó por SALIDA
            if (carta.valor == 0 || posicionAnterior > carta.valor) {
                cout << "🎯 Pasaste por SALIDA!" << endl;
                bancoPagarSalida(banco, jugador);
            }
            
            cout << "\nProcesando nueva casilla..." << endl;
            procesarCasilla(jugador);
        }
        else if (carta.accion == "RETROCEDER") {
            int posicionAnterior = jugador.posicion;
            jugador.posicion = (jugador.posicion - carta.valor + 40) % 40;
            
            cout << "↩️ Retrocedes " << carta.valor << " casillas (de " 
                 << posicionAnterior << " a " << jugador.posicion << ")" << endl;
            
            procesarCasilla(jugador);
        }
        else if (carta.accion == "SALIR_CARCEL") {
            darCartaSalirCarcel(jugador);
            cout << "🎴 ¡Guardaste la carta! Podrás usarla cuando estés en la cárcel." << endl;
            // Esta carta NO se devuelve a la cola
        }
        else if (carta.accion == "IR_CARCEL") {
            cout << "🚨 ¡Vas directamente a la CÁRCEL!" << endl;
            enviarACarcel(jugador);
        }
        else if (carta.accion == "REPARACIONES") {
            int totalCasas = 0;
            int totalHoteles = 0;
            
            // Contar casas y hoteles del jugador
            for (const string& nombreProp : jugador.propiedades) {
                Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
                if (casilla != nullptr) {
                    Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                    if (prop != nullptr) {
                        int numCasas = prop->getNumCasas();
                        if (numCasas < 5) {
                            totalCasas += numCasas;
                        } else {
                            totalHoteles++;
                        }
                    }
                }
            }
            
            bancoReparaciones(banco, jugador, carta.valor, carta.valorExtra, 
                             totalCasas, totalHoteles);
        }
        else if (carta.accion == "PAGAR_JUGADORES") {
            cout << "💸 Debes pagar $" << carta.valor << " a CADA jugador" << endl;
            bancoPagarATodos(banco, jugador, jugadores, carta.valor);
        }
        else if (carta.accion == "MOVER_FERROCARRIL") {
            cout << "🚂 Avanzas al ferrocarril más cercano..." << endl;
            int ferrocarriles[] = {5, 15, 25, 35};  // Posiciones de ferrocarriles
            
            int distanciaMin = 40;
            int casillaDestino = ferrocarriles[0];
            
            for (int ferro : ferrocarriles) {
                int distancia = (ferro - jugador.posicion + 40) % 40;
                if (distancia < distanciaMin) {
                    distanciaMin = distancia;
                    casillaDestino = ferro;
                }
            }
            
            int posicionAnterior = jugador.posicion;
            jugador.posicion = casillaDestino;
            
            cout << "Te mueves de " << posicionAnterior << " a " << casillaDestino << endl;
            
            if (posicionAnterior > casillaDestino) {
                bancoPagarSalida(banco, jugador);
            }
            
            procesarCasilla(jugador);
        }
        else if (carta.accion == "MOVER_SERVICIO") {
            cout << "⚡ Avanzas al servicio más cercano..." << endl;
            int servicios[] = {12, 28};  // Electric Company y Water Works
            
            int distanciaMin = 40;
            int casillaDestino = servicios[0];
            
            for (int serv : servicios) {
                int distancia = (serv - jugador.posicion + 40) % 40;
                if (distancia < distanciaMin) {
                    distanciaMin = distancia;
                    casillaDestino = serv;
                }
            }
            
            int posicionAnterior = jugador.posicion;
            jugador.posicion = casillaDestino;
            
            cout << "Te mueves de " << posicionAnterior << " a " << casillaDestino << endl;
            
            if (posicionAnterior > casillaDestino) {
                bancoPagarSalida(banco, jugador);
            }
            
            procesarCasilla(jugador);
        }
        else {
            cout << "⚠️ Acción de carta no implementada: " << carta.accion << endl;
        }
        
        cout << "\n💰 Dinero actual: $" << jugador.dinero << endl;
    }

    /**
     * Precondición: Jugador válido
     * Postcondición: Procesa completamente una carta de Cofre Comunitario
     */
    void procesarCartaCofre(Jugador& jugador) {
        cout << "\n🎴 ¡Sacaste una carta de COFRE COMUNITARIO!" << endl;

        Carta carta = sistemaCartas.sacarCartaCofre();

        // Verificar si la carta es válida
        if (carta.accion.empty()) {
            cout << "⚠️ No hay cartas disponibles en este momento" << endl;
            return;
        }

        mostrarCarta(carta);

        cout << "\nPresiona Enter para continuar...";
        cin.ignore();
        cin.get();

        // Procesar según el tipo de acción
        if (carta.accion == "COBRAR") {
            bancoOtorgarDinero(banco, jugador, carta.valor, carta.descripcion);
        }
        else if (carta.accion == "PAGAR") {
            if (!bancoCobrarDinero(banco, jugador, carta.valor, carta.descripcion)) {
                cout << "⚠️ No tienes suficiente dinero. Debes vender propiedades o declararte en quiebra." << endl;
            }
        }
        else if (carta.accion == "MOVER") {
            int posicionAnterior = jugador.posicion;
            jugador.posicion = carta.valor;
            
            cout << "🚶 Te mueves a la casilla " << carta.valor << " (SALIDA)" << endl;
            
            // Si es SALIDA (casilla 0), siempre cobra $200
            bancoPagarSalida(banco, jugador);
        }
        else if (carta.accion == "SALIR_CARCEL") {
            darCartaSalirCarcel(jugador);
            cout << "🎴 ¡Guardaste la carta! Podrás usarla cuando estés en la cárcel." << endl;
        }
        else if (carta.accion == "IR_CARCEL") {
            cout << "🚨 ¡Vas directamente a la CÁRCEL!" << endl;
            enviarACarcel(jugador);
        }
        else if (carta.accion == "COBRAR_JUGADORES") {
            cout << "💰 Cada jugador te paga $" << carta.valor << endl;
            bancoCobrarDeTodos(banco, jugador, jugadores, carta.valor);
        }
        else if (carta.accion == "REPARACIONES") {
            int totalCasas = 0;
            int totalHoteles = 0;
            
            // Contar casas y hoteles del jugador
            for (const string& nombreProp : jugador.propiedades) {
                Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
                if (casilla != nullptr) {
                    Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                    if (prop != nullptr) {
                        int numCasas = prop->getNumCasas();
                        if (numCasas < 5) {
                            totalCasas += numCasas;
                        } else {
                            totalHoteles++;
                        }
                    }
                }
            }
            
            bancoReparaciones(banco, jugador, carta.valor, carta.valorExtra, 
                             totalCasas, totalHoteles);
        }
        else {
            cout << "⚠️ Acción de carta no implementada: " << carta.accion << endl;
        }
        
        cout << "\n💰 Dinero actual: $" << jugador.dinero << endl;
    }
    
    /**
     * Precondición: Jugador válido, está en cárcel
     * Postcondición: Intenta salir de cárcel según reglas
     */
    void procesarCárcel(Jugador& jugador) {
        if (!jugador.enCarcel) {
            return;
        }
        
        cout << "\n🔒 " << jugador.nombre << " está en la CÁRCEL" << endl;
        cout << "Turnos: " << jugador.turnosCarcel << "/3" << endl;
        
        // Opción 1: Usar carta
        if (usarCartaSalirCarcel(jugador)) {
            cout << "✅ " << jugador.nombre << " salió gratis con su carta" << endl;
            return;
        }
        
        // Opción 2: Pagar multa
        cout << "\n¿Intentar salir pagando $50? (s/n): ";
        char respuesta;
        cin >> respuesta;
        
        if (respuesta == 's' || respuesta == 'S') {
            if (bancoMultaCarcel(banco, jugador)) {
                cout << "✅ Saliste de cárcel pagando multa" << endl;
                return;
            } else {
                cout << "❌ No tienes dinero para pagar la multa" << endl;
            }
        }
        
        // Opción 3: Esperar turno
        jugador.turnosCarcel++;
        cout << "⏰ Esperas en cárcel (turno " << jugador.turnosCarcel << "/3)" << endl;
        
        if (jugador.turnosCarcel >= 3) {
            jugador.enCarcel = false;
            jugador.turnosCarcel = 0;
            cout << "⏰ 3 turnos completados - Sales automáticamente" << endl;
        }
    }
    
    /**
     * Precondición: Turno válido
     * Postcondición: Ejecuta un turno completo del jugador actual
     */
    void procesarTurno() {
        if (turnoActual >= jugadores.size()) {
            turnoActual = 0;
            rondaActual++;
        }
        
        Jugador& jugadorActual = jugadores[turnoActual];
        
        if (jugadorActual.estaQuebrado) {
            cout << "\n⏭️ " << jugadorActual.nombre << " está quebrado - Turno saltado" << endl;
            turnoActual++;
            return;
        }
        
        // ===== INICIO DEL TURNO =====
        cout << "\n" << string(60, '=') << endl;
        cout << "🎯 RONDA " << rondaActual << " - TURNO DE: " << jugadorActual.nombre << endl;
        cout << string(60, '=') << endl;
        
        mostrarEstadoJugador(jugadorActual);
        
        // ===== PROCESAR CÁRCEL =====
        if (jugadorActual.enCarcel) {
            procesarCárcel(jugadorActual);
            turnoActual++;
            return;  // Turno termina si estaba en cárcel
        }

        // ===== MENÚ DE OPCIONES ANTES DE TIRAR =====
        bool continuarTurno = false;
        while (!continuarTurno) {
            cout << "\n+---------------------------------------+" << endl;
            cout << "|   QUE QUIERES HACER?                  |" << endl;
            cout << "+---------------------------------------+" << endl;
            cout << "| 1. [DADOS] Lanzar dados (continuar)  |" << endl;
            cout << "| 2. [CONSTRUIR] Construir casas        |" << endl;
            cout << "| 3. [PROP] Ver mis propiedades         |" << endl;
            cout << "+---------------------------------------+" << endl;
            cout << "Opcion: ";

            int opcion;
            cin >> opcion;

            if (opcion == 1) {
                continuarTurno = true;
            }
            else if (opcion == 2) {
                menuConstruccion(jugadorActual);
            }
            else if (opcion == 3) {
                cout << "\n🏠 === MIS PROPIEDADES ===" << endl;
                cout << "💰 Dinero: $" << jugadorActual.dinero << endl;
                cout << "🏘️  Total propiedades: " << jugadorActual.propiedades.size() << endl;

                if (jugadorActual.propiedades.empty()) {
                    cout << "❌ No tienes propiedades aún" << endl;
                } else {
                    cout << "\nLista de propiedades:" << endl;
                    for (const string& nombreProp : jugadorActual.propiedades) {
                        Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
                        if (casilla != nullptr) {
                            Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                            Ferrocarril* ferro = dynamic_cast<Ferrocarril*>(casilla);
                            Servicio* serv = dynamic_cast<Servicio*>(casilla);

                            if (prop != nullptr) {
                                cout << "  🏠 " << prop->getNombre()
                                     << " | Color: " << prop->getColor()
                                     << " | Casas: " << prop->getNumCasas()
                                     << " | Alquiler: $" << prop->obtenerAlquiler() << endl;
                            }
                            else if (ferro != nullptr) {
                                cout << "  🚂 " << ferro->getNombre() << endl;
                            }
                            else if (serv != nullptr) {
                                cout << "  ⚡ " << serv->getNombre() << endl;
                            }
                        }
                    }

                    // Mostrar monopolios
                    vector<string> monopolios = obtenerMonopolios(jugadorActual);
                    if (!monopolios.empty()) {
                        cout << "\n🎨 MONOPOLIOS COMPLETOS:" << endl;
                        for (const string& color : monopolios) {
                            cout << "  ✅ " << color << endl;
                        }
                    }
                }
                cout << "=========================" << endl;
            }
            else {
                cout << "❌ Opción inválida" << endl;
            }
        }

        // ===== LANZAR DADOS =====
        cout << "\n🎲 Lanzando dados...";
        cin.ignore();
        cin.get();
        
        lanzarDado(dado);
        cout << "SUMA: " << obtenerSuma(dado) << endl;
        
        if (esDoble(dado)) {
            cout << "🔄 ¡DOBLE! Lanzarás de nuevo" << endl;
        }
        
        // ===== MOVIMIENTO =====
        procesarMovimiento(jugadorActual, obtenerSuma(dado));
        
        // ===== PROCESAR CASILLA =====
        procesarCasilla(jugadorActual);
        
        // ===== SIGUIENTE TURNO =====
        turnoActual++;
    }
    

    /**
     * Precondición: Ninguna
     * Postcondición: Retorna true si el juego debe terminar
     */
    bool finDelJuego() const {
        int activos = 0;
        for (const Jugador& j : jugadores) {
            if (!j.estaQuebrado) {
                activos++;
            }
        }
        
        // Fin si solo queda 1 jugador O si se alcanzaron 50 rondas
        if (activos <= 1) {
            cout << "\n🏆 Solo queda un jugador en pie!" << endl;
            return true;
        }
        
        if (rondaActual > 50) {
            cout << "\n⏰ Se alcanzó el límite de 50 rondas!" << endl;
            return true;
        }
        
        return false;
    }
    
    /**
 * Precondición: Juego finalizado
 * Postcondición: Muestra ganador y estadísticas finales
 */
void finalizarJuego() {
    cout << "\n" << string(60, '=') << endl;
    cout << "🏆 ¡JUEGO TERMINADO!" << endl;
    cout << string(60, '=') << endl;
    
    // Calcular patrimonio de todos los jugadores activos
    vector<pair<string, int>> ranking;  // (nombre, patrimonio)
    
    for (const Jugador& j : jugadores) {
        if (!j.estaQuebrado) {
            int patrimonio = calcularPatrimonio(j);
            ranking.push_back(make_pair(j.nombre, patrimonio));
            
            cout << "\n💰 " << j.nombre << ":" << endl;
            cout << "   Dinero en efectivo: $" << j.dinero << endl;
            cout << "   Propiedades: " << j.propiedades.size() << endl;
            
            // Mostrar lista de propiedades
            if (j.propiedades.size() > 0) {
                cout << "   Lista de propiedades:" << endl;
                for (const string& prop : j.propiedades) {
                    Casilla* casilla = buscarPropiedadEnTablero(prop);
                    if (casilla != nullptr) {
                        Propiedad* p = dynamic_cast<Propiedad*>(casilla);
                        if (p != nullptr) {
                            cout << "     - " << prop << " ($" << p->getPrecio() 
                                 << ", " << p->getNumCasas() << " casas)" << endl;
                        } else {
                            cout << "     - " << prop << endl;
                        }
                    }
                }
            }
            
            cout << "   PATRIMONIO TOTAL: $" << patrimonio << endl;
        }
    }
    
    // Ordenar ranking por patrimonio (mayor a menor)
    sort(ranking.begin(), ranking.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    // Mostrar ranking
    cout << "\n" << string(60, '=') << endl;
    cout << "🏆 RANKING FINAL" << endl;
    cout << string(60, '=') << endl;
    
    for (size_t i = 0; i < ranking.size(); i++) {
        string medalla = (i == 0) ? "🥇" : (i == 1) ? "🥈" : (i == 2) ? "🥉" : "  ";
        cout << medalla << " " << (i + 1) << ". " << ranking[i].first 
             << " - $" << ranking[i].second << endl;
    }
    
    // Anunciar ganador
    if (ranking.size() > 0) {
        cout << "\n" << string(60, '=') << endl;
        cout << "🎉 ¡GANADOR: " << ranking[0].first << "!" << endl;
        cout << "💰 Patrimonio final: $" << ranking[0].second << endl;
        cout << string(60, '=') << endl;
    }
    
    // Mostrar resumen financiero del banco
    cout << endl;
    bancoResumenFinanciero(banco, jugadores);
    
    // Estadísticas del juego
    cout << "\n📊 ESTADÍSTICAS DEL JUEGO:" << endl;
    cout << "Rondas jugadas: " << rondaActual << endl;
    cout << "Jugadores quebrados: " << (jugadores.size() - ranking.size()) << endl;
    cout << string(60, '=') << endl;
}
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra el estado actual del juego
     */
    void mostrarEstadoActual() const {
        cout << "\n" << string(50, '-') << endl;
        cout << "📊 RONDA " << rondaActual << " | Turno: " << (turnoActual + 1) 
             << "/" << jugadores.size() << endl;
        cout << string(50, '-') << endl;
        
        cout << "\n📋 ESTADO DE JUGADORES:" << endl;
        for (size_t i = 0; i < jugadores.size(); i++) {
            const Jugador& j = jugadores[i];
            cout << (i == turnoActual ? "👉 " : "   ");
            cout << j.nombre << " - $" << j.dinero << " - Pos:" << j.posicion;
            if (j.enCarcel) cout << " [CÁRCEL]";
            if (j.estaQuebrado) cout << " [QUEBRADO]";
            cout << endl;
        }
        cout << string(50, '-') << endl;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Avanza al siguiente turno
     */
    void avanzarTurno() {
        turnoActual = (turnoActual + 1) % jugadores.size();
        if (turnoActual == 0) {
            rondaActual++;
        }
    }
    
public:
    /**
     * Precondición: Ninguna
     * Postcondición: Se crea un juego con estructuras inicializadas
     */
    Juego() : turnoActual(0), rondaActual(1), juegoActivo(false), turnosEnCarcelActual(0) {
        srand(time(0));
        banco = Banco();
        sistemaCartas = SistemaCartas();
    }
    
    /**
     * Precondición: Archivos de configuración deben existir
     * Postcondición: Inicializa todos los componentes del juego
     */
    bool inicializarJuego() {
        cout << "\n🎯 ===== MONOPOLY C++ =====" << endl;
        cout << "🎯 Proyecto Estructuras de Datos" << endl;
        cout << "🎯 =========================\n" << endl;
        
        // 1. Cargar tablero
        cout << "📋 Cargando tablero..." << endl;
        tablero = cargarTableroDesdeArchivo("src/Casillas.txt");  
        if (vaciaLista(tablero)) {
            cout << "❌ Error cargando tablero" << endl;
            return false;
        }
        cout << "✅ Tablero cargado (" << size(tablero) << " casillas)" << endl;
        
        // 2. Registrar propiedades (tabla hash)
        cout << "\n🏠 Registrando propiedades..." << endl;
        registrarPropiedades();
        
        // 3. Cargar cartas
        cout << "\n🎴 Cargando sistema de cartas..." << endl;
        cargarCartas();
        
        // 4. Configurar jugadores
        cout << "\n👥 Configurando jugadores..." << endl;
        configurarJugadores();
        
        
        juegoActivo = true;
        cout << "\n✅ ¡Juego inicializado correctamente!" << endl;
        return true;
    }
    
    /**
     * Precondición: Juego debe estar inicializado
     * Postcondición: Ejecuta el loop principal del juego
     */
    void jugar() {
        if (!juegoActivo) {
            cout << "❌ Juego no inicializado" << endl;
            return;
        }
        
        cout << "\n🎮 ¡COMENZANDO EL JUEGO!" << endl;
        cout << "Escribe 'salir' para terminar\n" << endl;
        
        while (juegoActivo && !finDelJuego()) {
            mostrarEstadoActual();
            
            cout << "\n¿Continuar? (s/n): ";
            char respuesta;
            cin >> respuesta;
            
            if (respuesta == 'n' || respuesta == 'N') {
                cout << "🎮 Juego interrumpido por el usuario" << endl;
                break;
            }
            
            procesarTurno();
            
        }
        
        if (finDelJuego()) {
            finalizarJuego();
        }
    }
    
    /**
     * Precondición: Juego debe estar inicializado
     * Postcondición: Muestra resumen del tablero
     */
    void mostrarTablero() const {
        if (vaciaLista(tablero)) {
            cout << "El tablero está vacío" << endl;
            return;
        }
        
        mostrarResumenTablero(tablero);
        mostrarEstadisticasTablero(tablero);
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna referencia al banco del juego
     */
    Banco& obtenerBanco() {
        return banco;
    }
    
    /**
     * Precondición: Ninguna
     * Postcondición: Retorna referencia a los jugadores del juego
     */
    vector<Jugador>& obtenerJugadores() {
        return jugadores;
    }

    /**
     * Precondición: nombrePropiedad no vacío
     * Postcondición: Retorna puntero a casilla o nullptr
     */
    Casilla* buscarPropiedadEnTablero(const string& nombrePropiedad) const {
        if (vaciaLista(tablero)) {
            return nullptr;
        }
        
        Casilla* actual = getCabeza(tablero);
        
        do {
            if (actual->getNombre() == nombrePropiedad) {
                return actual;
            }
            actual = actual->siguiente;
        } while (actual != getCabeza(tablero));
        
        return nullptr;
    }

    /**
     * Precondición: jugador válido
     * Postcondición: Retorna el patrimonio total (dinero + valor de propiedades)
     */
    int calcularPatrimonio(const Jugador& jugador) const {
        int patrimonio = jugador.dinero;
        
        // Sumar el valor de todas las propiedades del jugador
        for (const string& nombreProp : jugador.propiedades) {
            Casilla* casilla = buscarPropiedadEnTablero(nombreProp);
            
            if (casilla != nullptr) {
                // Intentar cast a diferentes tipos de propiedades
                Propiedad* prop = dynamic_cast<Propiedad*>(casilla);
                Ferrocarril* ferro = dynamic_cast<Ferrocarril*>(casilla);
                Servicio* serv = dynamic_cast<Servicio*>(casilla);
                
                if (prop != nullptr) {
                    patrimonio += prop->getPrecio();
                    // Bonus por casas/hoteles construidos
                    patrimonio += prop->getValorMejoras();
                }
                else if (ferro != nullptr) {
                    patrimonio += ferro->getPrecio();
                }
                else if (serv != nullptr) {
                    patrimonio += serv->getPrecio();
                }
            }
        }
        
        return patrimonio;
    }
    
    
    /**
     * Precondición: Ninguna
     * Postcondición: Muestra información del estado actual
     */
    void mostrarInfo() const {
        cout << "\n=== INFORMACIÓN DEL JUEGO ===" << endl;
        cout << "Ronda: " << rondaActual << endl;
        cout << "Turno actual: " << turnoActual + 1 << "/" << jugadores.size() << endl;
        cout << "Juego activo: " << (juegoActivo ? "SÍ" : "NO") << endl;
        cout << "Jugadores: " << jugadores.size() << endl;
        cout << "==============================" << endl;
    }


    
};

#endif // JUEGO_H_