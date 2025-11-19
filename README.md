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
- Sistema Windows con MinGW o Linux/Mac

COMPILAR Y EJECUTAR:

1. Dar permisos al script (solo primera vez):
   chmod +x compile.sh

2. Compilar:
   ./compile.sh

3. Ejecutar:
   ./monopoly

COMPILACIÓN MANUAL (alternativa):
   g++ -std=c++11 -o monopoly src/main.cpp src/listaCircular.cpp src/tablero.cpp -I./src -static-libgcc -static-libstdc++


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
  ├── Casillas.txt          - Datos del tablero
  └── cartas.txt            - Datos de cartas


ESTRUCTURAS DE DATOS IMPLEMENTADAS:
----------------------------------------------
✓ Lista Circular - Tablero de 40 casillas
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
- Manejo de memoria: Smart pointers donde aplica
- Separación header/implementation: listaCircular, tablero
- Documentación: Precondiciones y postcondiciones en código


MENÚ DEL PROGRAMA:
----------------------------------------------
1. Nuevo Juego - Inicia partida completa de Monopoly
2. Pruebas de TADs - Pruebas individuales de estructuras
   - Tabla Hash de Propiedades
   - Sistema de Cartas (Colas)
   - TAD Jugador
   - Carga de Tablero (Lista Circular)
3. Ver información del proyecto
4. Salir


NOTAS IMPORTANTES:
----------------------------------------------
- Los archivos de datos deben estar en src/
- Compilar con flags -static-libgcc -static-libstdc++ en Windows
- El tablero tiene 40 casillas (estándar de Monopoly)
- Sistema de cartas implementa correctamente la mecánica
  de devolver cartas al final de la cola


LIMITACIONES CONOCIDAS:
----------------------------------------------
- No se implementó sistema de subasta (simplificación pedida)
- Sistema de undo/pila preparado pero no integrado
- Lógica de monopolios de color pendiente de completar


AUTORES:
----------------------------------------------
Proyecto desarrollado como parte del curso de
Estructuras de Datos - Universidad del Valle
Octubre 2025

===============================================

./monopoly
```

Cuando veas el menú:
- Ingresa: `2` (Pruebas de TADs)
- Luego: `4` (Probar carga de Tablero)

**Deberías ver:**
```
🧪 === PRUEBA CARGA DE TABLERO (LISTA CIRCULAR) ===

--- Cargando tablero ---
✅ Tablero cargado (40 casillas)

--- Resumen del tablero ---
...
```

Si ves eso, **¡PERFECTO!** ✅

---

### 2️⃣ Prueba el sistema de cartas

Presiona Enter para volver al menú de pruebas, luego:
- Ingresa: `2` (Probar Sistema de Cartas)

**Deberías ver:**
```
🧪 === PRUEBA SISTEMA DE CARTAS (COLAS) ===

--- Inicializando sistema ---
🎴 Inicializando sistema de cartas...
✅ Cargadas X cartas de SUERTE
✅ Cargadas X cartas de COFRE
✅ Sistema de cartas listo