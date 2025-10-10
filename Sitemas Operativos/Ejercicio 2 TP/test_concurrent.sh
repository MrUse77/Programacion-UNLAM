#!/bin/bash

# Script para pruebas automatizadas de concurrencia y transacciones

echo "=========================================="
echo "Prueba Automatizada de Concurrencia"
echo "=========================================="
echo ""

PORT=8080
SERVER_HOST="127.0.0.1"

# Verificar que el servidor está corriendo
if ! lsof -Pi :$PORT -sTCP:LISTEN -t >/dev/null 2>&1; then
    echo "Error: El servidor no está corriendo en el puerto $PORT"
    echo "Ejecute primero: ./test_server.sh"
    exit 1
fi

echo "✓ Servidor detectado en puerto $PORT"
echo ""

# Función para ejecutar comandos en un cliente
test_client() {
    local client_id=$1
    local commands=$2
    local delay=$3
    
    echo "=== Cliente $client_id ===" > client${client_id}.log
    
    (
        sleep $delay
        echo "$commands" | ./client -s $SERVER_HOST -p $PORT >> client${client_id}.log 2>&1
    ) &
    
    return $!
}

echo "Prueba 1: Consultas concurrentes (sin transacción)"
echo "=================================================="
echo ""

# Tres clientes haciendo SELECT ALL al mismo tiempo
test_client 1 "SELECT ALL
QUIT" 0

test_client 2 "SELECT City Madrid
QUIT" 0.5

test_client 3 "SELECT Department IT
QUIT" 1

sleep 5

echo "Resultados:"
echo ""
echo "Cliente 1:"
tail -5 client1.log
echo ""
echo "Cliente 2:"
tail -5 client2.log
echo ""
echo "Cliente 3:"
tail -5 client3.log
echo ""

echo "Prueba 2: Transacción bloqueante"
echo "================================="
echo ""

# Cliente 1 inicia transacción y mantiene el lock
test_client 4 "BEGIN TRANSACTION
SELECT ALL
$(sleep 5)
UPDATE 1 Salary 99999
COMMIT TRANSACTION
QUIT" 0

sleep 2

# Cliente 2 intenta consultar mientras hay transacción activa
test_client 5 "SELECT ALL
QUIT" 0

sleep 6

echo "Resultados:"
echo ""
echo "Cliente 4 (con transacción):"
tail -10 client4.log
echo ""
echo "Cliente 5 (bloqueado):"
tail -5 client5.log
echo ""

echo "Prueba 3: Operaciones CRUD"
echo "=========================="
echo ""

# Insertar, actualizar y eliminar un registro
test_client 6 "INSERT 999,TestUser,30,TestCity,TestDept,60000,5
SELECT ID 999
UPDATE 999 Name UpdatedUser
SELECT ID 999
DELETE 999
SELECT ID 999
QUIT" 0

sleep 5

echo "Resultados:"
echo ""
echo "Cliente 6 (CRUD completo):"
cat client6.log | tail -30
echo ""

echo "=========================================="
echo "Pruebas completadas"
echo "=========================================="
echo ""
echo "Logs guardados en client*.log"
echo ""

# Limpiar logs antiguos (opcional)
# rm -f client*.log
