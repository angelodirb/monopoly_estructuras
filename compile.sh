#!/bin/bash
echo "🔨 Compilando Monopoly C++..."
echo ""

g++ -std=c++11  -o monopoly \
    src/main.cpp \
    src/listaCircular.cpp \
    src/tablero.cpp \
    -I./src \
    -static-libgcc -static-libstdc++

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ ¡Compilación exitosa!"
    echo ""
    echo "Para ejecutar el juego:"
    echo "  ./monopoly"
    echo ""
else
    echo ""
    echo "❌ Error de compilación"
    echo "Revisa los mensajes de error arriba"
    exit 1
fi