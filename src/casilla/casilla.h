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

    virtual void activar() = 0;
};