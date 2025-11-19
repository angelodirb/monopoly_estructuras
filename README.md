===============================================
    MONOPOLY C++ - PROYECTO FINAL
    Estructuras de Datos - Grupo B
    Profesor: Gerardo M. Sarria M.
===============================================

INTEGRANTES:
[Tu nombre]
[Nombre de tu compañero]

INSTRUCCIONES DE COMPILACIÓN:
----------------------------------------------

REQUISITOS:
- g++ con soporte para C++11 o superior
- Sistema Windows con MinGW, Linux o Mac

COMPILAR:

   g++ -std=c++11 -o monopoly src/main.cpp src/listaCircular.cpp src/tablero.cpp -I./src -static-libgcc -static-libstdc++

EJECUTAR:

   ./monopoly


ESTRUCTURA DEL PROYECTO:
----------------------------------------------
src/
  ├── main.cpp              - Punto de entrada y menú principal
  ├── Juego.h               - TAD Juego (sistema principal)
  ├── Banco.h               - TAD Banco
  ├── jugador.h             - TAD Jugador
  ├── Dado.h                - TAD Dado
  ├── tablero.h/.cpp        - Manejo del tablero
  ├── listaCircular.h/.cpp  - Lista circular para tablero
  ├── casilla.h             - Clase base polimórfica
  ├── propiedad.h           - Propiedades normales
  ├── ferrocarril.h         - Ferrocarriles
  ├── servicios.h           - Servicios públicos
  ├── tablaHash.h           - Tabla hash genérica
  ├── registroPropiedades.h - Registro de propiedades
  ├── cola.h                - TAD Cola genérico
  ├── ColaCartas.h          - Sistema de cartas
  ├── cartas.h              - TAD Carta
  ├── lista.h               - TAD Lista enlazada
  ├── pila.h                - TAD Pila
  ├── Casillas.txt          - Datos del tablero (40 casillas)
  └── cartas.txt            - Datos de cartas (32 cartas)


ESTRUCTURAS DE DATOS IMPLEMENTADAS:
----------------------------------------------
✓ Lista Circular - Tablero de 40 casillas con navegación cíclica
✓ Jerarquía Polimórfica - Casilla → Propiedad/Ferrocarril/Servicio
✓ Tabla Hash - Registro de propiedades con hashing de strings
✓ Cola - Sistema de cartas (Suerte y Cofre Comunitario)
✓ Lista Enlazada - Base para otras estructuras
✓ Pila - Preparada para sistema de undo (opcional)


FUNCIONALIDADES PRINCIPALES:
----------------------------------------------
✓ Carga dinámica del tablero desde archivo
✓ Sistema de cartas con colas FIFO
✓ Registro eficiente de propiedades (O(1) búsqueda)
✓ Manejo completo del banco (transacciones, multas, reparaciones)
✓ Sistema de jugadores con estado completo
✓ Dados con detección de dobles
✓ Mecánica de cárcel
✓ Interfaz de línea de comandos
✓ Pre/postcondiciones documentadas en cada función


CARACTERÍSTICAS TÉCNICAS:
----------------------------------------------
- Lenguaje: C++11
- Paradigma: Programación orientada a objetos
- Polimorfismo: Jerarquía de casillas
- Separación header/implementation: listaCircular, tablero
- Documentación: Precondiciones y postcondiciones en código
- Compilación: Flags estáticos para compatibilidad Windows/Linux


MENÚ DEL PROGRAMA:
----------------------------------------------
1. Nuevo Juego - Inicia partida completa de Monopoly
   - Configuración de número de jugadores
   - Selección de fichas
   - Sistema completo de turnos

2. Pruebas de TADs - Pruebas individuales de estructuras
   - Tabla Hash de Propiedades
   - Sistema de Cartas (Colas)
   - TAD Jugador
   - Carga de Tablero (Lista Circular)

3. Ver información del proyecto

4. Salir


FORMATO DE ARCHIVOS DE DATOS:
----------------------------------------------

Casillas.txt:
  NOMBRE_CASILLA
  TIPO
  [datos específicos del tipo]
  ===

cartas.txt:
  Descripción de la carta
  TIPO (SUERTE o COFRE)
  ACCION
  VALOR
  [VALOR_EXTRA opcional]
  ===


CORRECCIONES REALIZADAS:
----------------------------------------------
✓ Ajustado formato de lectura del tablero (sin campo color)
✓ Corregido nombre de archivo de includes (RegistroPropiedades.h)
✓ Verificada carga correcta de 40 casillas
✓ Sistema de cartas con debugging implementado


NOTAS IMPORTANTES:
----------------------------------------------
- Los archivos de datos están en src/
- Compilar con flags -static-libgcc -static-libstdc++ en Windows
- El tablero tiene 40 casillas (estándar de Monopoly)
- Sistema de cartas implementa correctamente la mecánica
  de devolver cartas al final de la cola
- Ejecutar desde la raíz del proyecto (no desde src/)


LIMITACIONES CONOCIDAS:
----------------------------------------------
- No se implementó sistema de subasta (simplificación pedida)
- Sistema de undo/pila preparado pero no integrado
- Lógica de monopolios de color pendiente de completar


VERIFICACIÓN DE FUNCIONAMIENTO:
----------------------------------------------
Para verificar que el proyecto funciona correctamente:

1. Compilar el proyecto (comando arriba)
2. Ejecutar: ./monopoly
3. Seleccionar opción 2 (Pruebas de TADs)
4. Seleccionar opción 4 (Probar carga de Tablero)
   - Debe cargar 40 casillas correctamente
5. Volver al menú y seleccionar opción 2 (Probar Sistema de Cartas)
   - Debe cargar cartas de SUERTE y COFRE
6. Seleccionar opción 3 (Probar TAD Jugador)
   - Debe mostrar operaciones de jugadores


AUTORES:
----------------------------------------------
Proyecto desarrollado como parte del curso de
Estructuras de Datos - Universidad del Valle
Noviembre 2025

===============================================