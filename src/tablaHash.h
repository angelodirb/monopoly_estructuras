#ifndef __TABLA_HASH
#define __TABLA_HASH

#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

// **********************************
// Estructuras de datos
// **********************************
template <typename L, typename T>
struct Dato {
    L llave;
    T elemento;
    bool ocupado;    // Para saber si está ocupado
    bool eliminado;  // Para manejar eliminaciones
};

template <typename L, typename T>
struct OA {
    int capacidad;
    int tamano;      // Contador de elementos
    struct Dato<L,T> *datos;
};

template <typename L, typename T>
using TablaHash = OA<L,T>*;

// **********************************
// Operaciones
// **********************************

// crearTablaHash: int -> TablaHash
template <typename L, typename T>
TablaHash<L,T> crearTablaHash(int c) {
    TablaHash<L,T> TH = (TablaHash<L,T>)malloc(sizeof(OA<L,T>));
    TH->capacidad = c;
    TH->tamano = 0;
    TH->datos = (Dato<L,T> *)malloc(c * sizeof(Dato<L,T>));
    
    // Inicializar todos los slots
    for (int i = 0; i < c; i++) {
        TH->datos[i].ocupado = false;
        TH->datos[i].eliminado = false;
    }
    
    return TH;
}

// Función hash para strings
inline int funcionHashString(const char* llave, int capacidad) {
    int hash = 0;
    int len = strlen(llave);
    for (int i = 0; i < len; i++) {
        hash = (hash * 31 + llave[i]) % capacidad;
    }
    return hash < 0 ? hash + capacidad : hash;
}

// Función hash genérica
template <typename L>
int funcionHash(L llave, int capacidad) {
    return ((int)llave) % capacidad;
}

// Especialización para const char*
template<>
inline int funcionHash<const char*>(const char* llave, int capacidad) {
    return funcionHashString(llave, capacidad);
}

// Función para buscar slot (maneja colisiones)
template <typename L, typename T>
int buscarSlot(TablaHash<L,T> TH, L llave, bool paraInsertar = false) {
    int ubicacion = funcionHash(llave, TH->capacidad);
    int inicio = ubicacion;
    
    do {
        if (!TH->datos[ubicacion].ocupado) {
            if (paraInsertar || !TH->datos[ubicacion].eliminado) {
                return ubicacion;
            }
        }
        else if (strcmp((const char*)TH->datos[ubicacion].llave, (const char*)llave) == 0) {
            return ubicacion;
        }
        
        ubicacion = (ubicacion + 1) % TH->capacidad;
    } while (ubicacion != inicio);
    
    return -1;
}

// insTablaHash: TablaHash x L x T -> TablaHash
template <typename L, typename T>
TablaHash<L,T> insTablaHash(TablaHash<L,T> TH, L llave, T elemento) {
    if (TH->tamano >= TH->capacidad * 0.7) {
        cout << "⚠️ Advertencia: Tabla hash casi llena" << endl;
    }
    
    int slot = buscarSlot(TH, llave, true);
    if (slot == -1) {
        cout << "❌ Error: Tabla hash llena" << endl;
        return TH;
    }
    
    if (!TH->datos[slot].ocupado) {
        TH->tamano++;
    }
    
    TH->datos[slot].llave = llave;
    TH->datos[slot].elemento = elemento;
    TH->datos[slot].ocupado = true;
    TH->datos[slot].eliminado = false;
    
    return TH;
}

// infoTablaHash: TablaHash x L -> T
template <typename L, typename T>
T infoTablaHash(TablaHash<L,T> TH, L llave) {
    int slot = buscarSlot(TH, llave, false);
    if (slot != -1 && TH->datos[slot].ocupado && !TH->datos[slot].eliminado) {
        return TH->datos[slot].elemento;
    }
    
    T valorDefecto = T();
    return valorDefecto;
}

// existeTablaHash: TablaHash x L -> bool
template <typename L, typename T>
bool existeTablaHash(TablaHash<L,T> TH, L llave) {
    int slot = buscarSlot(TH, llave, false);
    return (slot != -1 && TH->datos[slot].ocupado && !TH->datos[slot].eliminado);
}

// elimTablaHash: TablaHash x L -> TablaHash
template <typename L, typename T>
TablaHash<L,T> elimTablaHash(TablaHash<L,T> TH, L llave) {
    int slot = buscarSlot(TH, llave, false);
    if (slot != -1 && TH->datos[slot].ocupado && !TH->datos[slot].eliminado) {
        TH->datos[slot].ocupado = false;
        TH->datos[slot].eliminado = true;
        TH->tamano--;
        cout << "✅ Elemento eliminado de tabla hash" << endl;
    }
    return TH;
}

// tamanoTablaHash: TablaHash -> int
template <typename L, typename T>
int tamanoTablaHash(TablaHash<L,T> TH) {
    return TH->tamano;
}

// estadisticasTablaHash: TablaHash -> void
template <typename L, typename T>
void estadisticasTablaHash(TablaHash<L,T> TH) {
    cout << "\n=== ESTADÍSTICAS TABLA HASH ===" << endl;
    cout << "Capacidad: " << TH->capacidad << endl;
    cout << "Elementos: " << TH->tamano << endl;
    cout << "Factor de carga: " << (double)TH->tamano / TH->capacidad * 100 << "%" << endl;
    cout << "===============================" << endl;
}

// liberarTablaHash: TablaHash -> void
template <typename L, typename T>
void liberarTablaHash(TablaHash<L,T> TH) {
    if (TH != NULL) {
        if (TH->datos != NULL) {
            free(TH->datos);
        }
        free(TH);
    }
}

#endif