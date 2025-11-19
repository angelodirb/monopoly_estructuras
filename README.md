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

# Sistema de Undo/Deshacer - Monopoly C++

## Descripción General

Se ha implementado un sistema completo de **undo/deshacer** que permite a los jugadores revertir acciones realizadas durante el juego, cumpliendo con el requisito de la rúbrica:

> "Vamos a asumir que un jugador puede dar marcha atrás a una jugada que acaba de realizar, de manera que se debe tener en una pila los estados del juego."

## Estructuras de Datos Implementadas

### 1. EstadoJuego.h

Estructura principal que captura el estado completo del juego en un momento dado:

```cpp
struct EstadoJuego {
    vector<Jugador> jugadores;           // Estado de todos los jugadores
    vector<EstadoPropiedad> propiedades; // Estado de propiedades del tablero
    int turnoActual;                     // Turno del jugador actual
    int rondaActual;                     // Ronda actual del juego
    int dineroEnCirculacion;             // Dinero del banco
    string descripcionAccion;            // Descripción de la acción guardada
};
```

### 2. EstadoPropiedad

Estructura auxiliar para guardar el estado de cada propiedad:

```cpp
struct EstadoPropiedad {
    string nombre;      // Nombre de la propiedad
    string duenio;      // Dueño actual (vacío si no tiene dueño)
    int numCasas;       // Número de casas/hotel construidas
};
```

### 3. Pila de Estados

Utiliza el TAD Pila implementado previamente en `pila.h`:

```cpp
Pila<EstadoJuego> pilaEstados;  // Pila LIFO para guardar estados
```

## Funciones Implementadas

### Funciones Privadas de la Clase Juego

#### 1. `capturarEstadoActual(descripcion)`

**Precondición:** Juego inicializado con jugadores y tablero
**Postcondición:** Retorna un EstadoJuego con el estado actual completo

Captura:
- Estado de todos los jugadores (dinero, posición, propiedades, etc.)
- Estado de todas las propiedades (dueño, casas)
- Estado de ferrocarriles y servicios
- Turno y ronda actual
- Dinero en circulación del banco

#### 2. `restaurarEstado(estado)`

**Precondición:** Estado válido capturado previamente
**Postcondición:** Restaura el juego al estado guardado

Restaura:
- Todos los jugadores a su estado anterior
- Dueños y casas de todas las propiedades
- Turno y ronda
- Dinero del banco

#### 3. `guardarEstado(descripcion)`

**Precondición:** Ninguna
**Postcondición:** Guarda el estado actual en la pila

- Captura el estado actual
- Lo agrega a la pila de estados
- Muestra mensaje confirmatorio con descripción

#### 4. `hayEstadosGuardados()`

**Precondición:** Ninguna
**Postcondición:** Retorna true si hay estados para deshacer

Verifica si la pila de estados no está vacía.

#### 5. `deshacerUltimaAccion()`

**Precondición:** Hay al menos un estado en la pila
**Postcondición:** Deshace la última acción restaurando el estado previo

- Obtiene el último estado de la pila
- Restaura el juego a ese estado
- Elimina el estado de la pila
- Retorna true si fue exitoso

## Integración en el Juego

### Puntos de Guardado Automático

El sistema guarda el estado automáticamente **ANTES** de las siguientes acciones:

1. **Antes de lanzar dados**
   - Descripción: "Antes de lanzar dados - Turno de [JUGADOR]"
   - Permite deshacer todo el turno si algo sale mal

2. **Antes de construir casas**
   - Descripción: "Antes de construir - [JUGADOR]"
   - Permite revertir construcciones erróneas

### Menú del Turno

Se agregó la opción **4. Deshacer** en el menú principal del turno:

```
┌─────────────────────────────────────┐
│   ¿QUE QUIERES HACER?              │
├─────────────────────────────────────┤
│ 1. Lanzar dados (continuar)        │
│ 2. Construir casas                 │
│ 3. Ver mis propiedades             │
│ 4. [UNDO] Deshacer última acción   │  ← NUEVO
└─────────────────────────────────────┘
💾 Estados guardados disponibles para deshacer
```

## Flujo de Uso

### Ejemplo de Uso

1. **Jugador en su turno:**
   - Ve el menú con opciones
   - Si hay estados guardados, se muestra "💾 Estados guardados disponibles"

2. **Jugador lanza dados:**
   - Sistema guarda estado actual ANTES de lanzar
   - Jugador tira dados y se mueve
   - Si cae en una propiedad y la compra por error...

3. **Jugador deshace:**
   - Selecciona opción 4 (Deshacer)
   - Sistema restaura el estado previo
   - Jugador vuelve a su posición y dinero anteriores
   - Propiedad queda sin dueño nuevamente

## Salida del Sistema

### Al Guardar Estado

```
[PILA] Estado guardado: Antes de lanzar dados - Turno de SOMBRERO (Total estados: 3)
```

### Al Deshacer

```
[UNDO] Restaurando estado: Antes de lanzar dados - Turno de SOMBRERO
[OK] Estado restaurado a Ronda 2, Turno 1
[OK] ¡Acción deshecha! Volviendo al menú...
```

### Si No Hay Estados

```
[ERROR] No hay estados guardados para deshacer
```

## Limitaciones y Consideraciones

### Límite de Estados

- Se define `MAX_ESTADOS = 20` para evitar saturar memoria
- Actualmente la pila puede crecer indefinidamente (simplificación)
- En una implementación completa, se eliminarían estados antiguos al exceder el límite

### Acciones que NO Guardan Estado

Por diseño, solo se guarda estado en acciones **reversibles**:
- Ver propiedades (opción 3) NO guarda estado
- Deshacer (opción 4) NO guarda estado

### Estado de Cartas

**NOTA IMPORTANTE:** El estado de las colas de cartas (Suerte y Cofre) NO se guarda.
Esto es una simplificación del sistema. Si un jugador deshace después de sacar una carta,
la carta ya salió de la cola y no se puede restaurar fácilmente.

## Complejidad

### Tiempo

- **Guardar estado:** O(n + m) donde n = jugadores, m = propiedades
- **Restaurar estado:** O(n + m)
- **Deshacer acción:** O(n + m)

### Espacio

- **Cada estado:** O(n + m)
- **Pila de estados:** O(k × (n + m)) donde k = número de estados guardados

## Cumplimiento de la Rúbrica

✅ **Estructura de Pila implementada:** Usa TAD Pila genérico con templates
✅ **Estados del juego guardados:** EstadoJuego captura todo el estado
✅ **Operación de undo funcional:** Sistema completo de deshacer
✅ **Integrado en la interfaz:** Opción 4 en el menú del turno
✅ **Precondiciones/Postcondiciones:** Todas las funciones documentadas

## Archivos Modificados/Creados

### Nuevos Archivos

1. **src/EstadoJuego.h** - Estructuras y funciones para estados
2. **SISTEMA_UNDO.md** - Esta documentación

### Archivos Modificados

1. **src/Juego.h**
   - Agregada pila de estados
   - Funciones de captura/restauración
   - Opción deshacer en menú
   - Guardado automático antes de acciones

2. **src/propiedad.h**
   - Agregado `setNumCasas()` para restaurar casas

## Pruebas Sugeridas

### Test 1: Deshacer Compra de Propiedad

1. Iniciar juego con 2 jugadores
2. Jugador 1 tira dados
3. Cae en propiedad y la compra
4. Ver dinero y propiedades (debería tener 1 menos dinero, 1 propiedad)
5. Seleccionar opción 4 (Deshacer)
6. Verificar que volvió a su estado anterior

### Test 2: Deshacer Construcción

1. Obtener monopolio de un color
2. Construir una casa
3. Ver que se descuenta dinero y aumenta alquiler
4. Deshacer
5. Verificar que la casa desaparece y recupera el dinero

### Test 3: Múltiples Deshacer

1. Realizar varias acciones consecutivas
2. Deshacer múltiples veces
3. Verificar que cada deshacer retrocede una acción

## Conclusión

El sistema de undo/deshacer está completamente implementado y funcional, cumpliendo con el requisito de la rúbrica de usar una pila para guardar estados del juego y permitir revertir acciones.