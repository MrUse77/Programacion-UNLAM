#!/bin/bash

# Script de prueba del sistema cliente-servidor

echo "==================================="
echo "Sistema Cliente-Servidor DB"
echo "Script de Prueba y Monitoreo"
echo "==================================="
echo ""

# Función para limpiar al salir
cleanup() {
    echo ""
    echo "Limpiando procesos..."
    pkill -f "./server"
    pkill -f "./client"
    sleep 1
    echo "Limpieza completada"
    exit 0
}

trap cleanup SIGINT SIGTERM

# Verificar que los ejecutables existen
if [ ! -f "./server" ] || [ ! -f "./client" ]; then
    echo "Error: Los ejecutables no existen. Ejecute 'make' primero."
    exit 1
fi

# Verificar que existe database.csv
if [ ! -f "./database.csv" ]; then
    echo "Error: No se encuentra database.csv"
    exit 1
fi

# Puerto de prueba
PORT=8080

echo "1. Verificando que el puerto $PORT está libre..."
if lsof -Pi :$PORT -sTCP:LISTEN -t >/dev/null 2>&1; then
    echo "   ⚠ Puerto $PORT en uso. Limpiando..."
    lsof -ti:$PORT | xargs kill -9 2>/dev/null
    sleep 2
fi
echo "   ✓ Puerto $PORT disponible"
echo ""

echo "2. Iniciando servidor en puerto $PORT..."
./server -p $PORT -n 3 -m 5 > server.log 2>&1 &
SERVER_PID=$!
sleep 2

if ! ps -p $SERVER_PID > /dev/null; then
    echo "   ✗ Error al iniciar el servidor"
    cat server.log
    exit 1
fi
echo "   ✓ Servidor iniciado (PID: $SERVER_PID)"
echo ""

echo "3. Información del servidor:"
echo "   - Proceso:"
ps aux | grep "./server" | grep -v grep | head -1
echo ""
echo "   - Socket:"
netstat -tlnp 2>/dev/null | grep ":$PORT" || ss -tlnp 2>/dev/null | grep ":$PORT"
echo ""
echo "   - Threads:"
ps -eLf | grep "./server" | grep -v grep | wc -l
echo ""

echo "4. Esperando 3 segundos..."
sleep 3
echo ""

echo "5. Estado del servidor:"
if lsof -Pi :$PORT -sTCP:LISTEN -t >/dev/null 2>&1; then
    echo "   ✓ Servidor escuchando en puerto $PORT"
else
    echo "   ✗ Servidor no está escuchando"
    cat server.log
    cleanup
fi
echo ""

echo "==================================="
echo "Servidor listo para recibir conexiones"
echo "==================================="
echo ""
echo "Para probar el cliente, ejecute en otras terminales:"
echo "   ./client -s 127.0.0.1 -p $PORT"
echo ""
echo "Comandos de ejemplo:"
echo "   SELECT ALL"
echo "   SELECT City Madrid"
echo "   INSERT 500,TestUser,25,TestCity,IT,50000,3"
echo "   UPDATE 500 Salary 55000"
echo "   DELETE 500"
echo "   BEGIN TRANSACTION"
echo "   COMMIT TRANSACTION"
echo ""
echo "Para monitorear el servidor:"
echo "   watch -n 1 'netstat -tan | grep $PORT'"
echo "   htop -p $SERVER_PID"
echo "   lsof -p $SERVER_PID"
echo ""
echo "Presione Ctrl+C para detener el servidor"
echo ""

# Mantener el script corriendo
tail -f server.log
