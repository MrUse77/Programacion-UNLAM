#!/bin/bash

# Configuraciones

# Nombre identificable del programa (puede contener espacios)
PROGRAMA="Servidor"

# Buscar PID del proceso padre (se ignora la línea del grep en sí)
PID_SERVIDOR=$(ps -eo pid,cmd | grep "$PROGRAMA" | grep -v grep | awk '{print $1}')
SERVIDOR="./Servidor/bin/Release/Servidor"
IP_LOCAL="127.0.0.1"
PUERTO="8080"
LOG="monitoreo_vivo.log"
INTERVALO=2 # segundos

# Control-C => limpieza
function limpiar() {
  echo -e "\n[INFO] Terminando servidor y limpiando recursos..."
  kill "$PID_SERVIDOR" 2>/dev/null
  wait "$PID_SERVIDOR" 2>/dev/null
  echo "[INFO] Servidor detenido."
  echo "[INFO] Verificando sockets restantes..."
  lsof -iTCP:$PUERTO -sTCP:LISTEN -nP
  exit 0
}

trap limpiar SIGINT

# Monitoreo en vivo
while true; do
  clear
  echo "===== MONITOREO EN VIVO - $(date) ====="
  echo "Servidor PID: $PID_SERVIDOR"
  echo -e "\n THREADS DEL SERVIDOR:"
  ps -eLf | grep "$PID_SERVIDOR" | grep -v grep

  echo -e "\n SOCKETS ABIERTOS:"
  ss -tnp | grep "$PUERTO" || echo "No hay sockets activos en el puerto $PUERTO"

  echo -e "\n CONEXIONES DEL SERVIDOR:"
  lsof -iTCP:$PUERTO -nP | grep LISTEN || echo "Ninguna conexión LISTEN detectada"

  echo -e "\n(Actualizando cada $INTERVALO segundos... Ctrl+C para salir)"
  sleep "$INTERVALO"
done
