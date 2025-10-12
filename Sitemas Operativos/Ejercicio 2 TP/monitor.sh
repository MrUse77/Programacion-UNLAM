#!/bin/bash

# Script de monitoreo del sistema

echo "=========================================="
echo "Monitor del Sistema Cliente-Servidor"
echo "=========================================="
echo ""

PORT=8080

if ! lsof -Pi :$PORT -sTCP:LISTEN -t >/dev/null 2>&1; then
    echo "Error: El servidor no está corriendo en el puerto $PORT"
    exit 1
fi

while true; do
    clear
    echo "=========================================="
    echo "Monitor del Sistema (Ctrl+C para salir)"
    echo "$(date)"
    echo "=========================================="
    echo ""
    
    echo "1. PROCESOS DEL SERVIDOR"
    echo "------------------------"
    ps aux | head -1
    ps aux | grep "./server" | grep -v grep
    echo ""
    
    echo "2. THREADS DEL SERVIDOR"
    echo "----------------------"
    SERVER_PID=$(pgrep -f "./server")
    if [ ! -z "$SERVER_PID" ]; then
        echo "PID del servidor: $SERVER_PID"
        echo "Número de threads: $(ps -eLf | grep -w $SERVER_PID | grep -v grep | wc -l)"
        ps -eLf | head -1
        ps -eLf | grep -w $SERVER_PID | grep -v grep | head -10
    else
        echo "Servidor no encontrado"
    fi
    echo ""
    
    echo "3. CONEXIONES DE RED"
    echo "-------------------"
    echo "Puerto $PORT:"
    netstat -tan 2>/dev/null | grep ":$PORT" || ss -tan 2>/dev/null | grep ":$PORT"
    echo ""
    echo "Conexiones establecidas: $(netstat -tan 2>/dev/null | grep ":$PORT" | grep ESTABLISHED | wc -l)"
    echo ""
    
    echo "4. ARCHIVOS ABIERTOS"
    echo "-------------------"
    if [ ! -z "$SERVER_PID" ]; then
        lsof -p $SERVER_PID 2>/dev/null | head -20
    fi
    echo ""
    
    echo "5. USO DE RECURSOS"
    echo "------------------"
    if [ ! -z "$SERVER_PID" ]; then
        ps -p $SERVER_PID -o pid,ppid,%cpu,%mem,vsz,rss,stat,time,cmd 2>/dev/null
    fi
    echo ""
    
    echo "Actualizando en 2 segundos..."
    sleep 2
done
