#!/bin/bash

# 1. Ejecutá el programa en segundo plano y capturá la salida
./ejercicio_1_tp >salida.txt &
main_pid=$!

# 2. Esperá a que el hijo imprima su PID
sleep 10

# 3. Extraé el PID del hijo desde el output
child_pid=$(grep "\[Hijo 3\]" salida.txt | awk -F 'PID=' '{print $2}' | awk -F ',' '{print $1}')

if [ -n "$child_pid" ]; then
  echo "Ejecutando strace sobre PID $child_pid"
  sudo strace -p "$child_pid"
else
  echo "No se pudo encontrar el PID del hijo"
fi
