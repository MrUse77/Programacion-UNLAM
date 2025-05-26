#!/bin/bash

# Nombre identificable del programa (puede contener espacios)
PROGRAMA="Ejercicio 1 TP"

# Buscar PID del proceso padre (se ignora la línea del grep en sí)
PID_PADRE=$(ps -eo pid,cmd | grep "$PROGRAMA" | grep -v grep | awk '{print $1}')

# Validar si se encontró el PID
if [ -z "$PID_PADRE" ]; then
  echo "❌ No se encontró ningún proceso llamado \"$PROGRAMA\""
  exit 1
fi

echo "✅ Programa \"$PROGRAMA\" encontrado con PID: $PID_PADRE"
echo

# Loop de monitoreo
while true; do
  echo "===================== MONITOREO EN VIVO ====================="
  echo "🧠 Memoria compartida (ipcs -m):"
  ipcs -m
  echo

  echo "🔒 Semáforos (ipcs -s):"
  ipcs -s
  echo

  echo "👨‍👧 Hijos del proceso PID 92469:"
  ps -eo pid,ppid,cmd | awk -v ppid="92469" '$2 == ppid'
  echo

  echo "🔁 Refrescando en 5 segundos... (Ctrl+C para salir)"
  sleep 5
  clear
done
