#ifndef CASILLA_H_
#define CASILLA_H_

#include <iostream>
#include <string>
using namespace std;

class Casilla {
protected:
    string nombre;
public:
    Casilla* siguiente; 
    
    Casilla(string n) : nombre(n), siguiente(nullptr) {}
    virtual ~Casilla() {}

    string getNombre() const { return nombre; }

    // Método virtual para comportamientos polimórficos
    virtual void activar() {
        cout << "Has caido en " << nombre << "." << endl;
    }
};

#endif
