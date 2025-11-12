#!/bin/bash

echo "🎲 Script de prueba para Monopoly C++"
echo "=================================="

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Función para imprimir con colores
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✅ $2${NC}"
    else
        echo -e "${RED}❌ $2${NC}"
    fi
}

echo -e "${YELLOW}Verificando estructura de archivos...${NC}"

# Verificar archivos necesarios
required_files=(
    "main.cpp"
    "tablero.cpp" 
    "tablero.h"
    "listaCircular.h"
    "Casillas.txt"
    "casilla/casilla.h"
    "casilla/propiedad.h"
    "casilla/ferrocarril.h"
    "casilla/servicios.h"
)

missing_files=0
for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "  ✅ $file"
    else
        echo "  ❌ $file (FALTANTE)"
        missing_files=$((missing_files + 1))
    fi
done

if [ $missing_files -gt 0 ]; then
    echo -e "${RED}Error: Faltan $missing_files archivos necesarios${NC}"
    exit 1
fi

echo -e "\n${YELLOW}Intentando compilar...${NC}"

# Intentar compilación
g++ -std=c++11 -Wall -Wextra -g -I . -I casilla main.cpp tablero.cpp -o monopoly_test 2> compile_errors.txt

if [ $? -eq 0 ]; then
    print_status 0 "Compilación exitosa"
    
    # Verificar si el archivo de datos existe
    if [ -f "Casillas.txt" ]; then
        echo -e "\n${YELLOW}Ejecutando prueba...${NC}"
        ./monopoly_test
        
        if [ $? -eq 0 ]; then
            print_status 0 "Ejecución exitosa"
        else
            print_status 1 "Error durante la ejecución"
        fi
    else
        echo -e "${YELLOW}Advertencia: Casillas.txt no encontrado, no se puede probar la ejecución${NC}"
    fi
    
    # Limpiar
    rm -f monopoly_test
else
    print_status 1 "Error de compilación"
    echo -e "\n${RED}Errores de compilación:${NC}"
    cat compile_errors.txt
fi

# Limpiar archivo de errores
rm -f compile_errors.txt

echo -e "\n${YELLOW}Prueba completada${NC}"
