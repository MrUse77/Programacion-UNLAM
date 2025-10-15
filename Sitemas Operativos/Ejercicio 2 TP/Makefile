# Makefile para el sistema cliente-servidor de base de datos

# Directorios
SRCDIR = src
BINDIR = bin

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

# Archivos fuente
SERVER_SRC = $(SRCDIR)/server.c
CLIENT_SRC = $(SRCDIR)/client.c

# Ejecutables
SERVER_BIN = $(BINDIR)/server
CLIENT_BIN = $(BINDIR)/client

# Regla por defecto
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compilar servidor
$(SERVER_BIN): $(SERVER_SRC) | $(BINDIR)
	@echo "Compilando servidor..."
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC) $(LDFLAGS)
	@echo "Servidor compilado: $(SERVER_BIN)"

# Compilar cliente
$(CLIENT_BIN): $(CLIENT_SRC) | $(BINDIR)
	@echo "Compilando cliente..."
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC) $(LDFLAGS)
	@echo "Cliente compilado: $(CLIENT_BIN)"

# Crear directorio bin si no existe
$(BINDIR):
	@echo "Creando directorio $(BINDIR)..."
	@mkdir -p $(BINDIR)

# Limpiar archivos compilados y temporales
clean:
	@echo "Limpiando archivos compilados..."
	@rm -f $(SERVER_BIN) $(CLIENT_BIN)
	@rm -f *.o
	@rm -f database.tmp
	@rm -f *.log
	@echo "Limpieza completada"

# Limpiar todo incluyendo backups y directorios
distclean: clean
	@echo "Limpieza completa..."
	@rm -f *~
	@rm -f $(SRCDIR)/*~
	@rm -f database.csv.bak
	@rm -rf $(BINDIR)
	@echo "Limpieza completa finalizada"

# Parámetros configurables para ejecución
PORT = 8080
MAX_CLIENTS = 5
MAX_QUEUE = 10
SERVER_HOST = 127.0.0.1

# Ejecutar servidor con parámetros configurables
run-server: $(SERVER_BIN)
	@echo "Iniciando servidor en puerto $(PORT)..."
	@echo "  Max clientes: $(MAX_CLIENTS)"
	@echo "  Max cola: $(MAX_QUEUE)"
	@echo ""
	./$(SERVER_BIN) -p $(PORT) -n $(MAX_CLIENTS) -m $(MAX_QUEUE)

# Ejecutar cliente conectándose al servidor
run-client: $(CLIENT_BIN)
	@echo "Conectando cliente a $(SERVER_HOST):$(PORT)..."
	@echo ""
	./$(CLIENT_BIN) -s $(SERVER_HOST) -p $(PORT)

# Recompilar completamente
rebuild: clean all
	@echo "Recompilación completa finalizada"

# Mostrar información del proyecto
info:
	@echo "=== Información del Proyecto ==="
	@echo ""
	@echo "Proyecto: Sistema Cliente-Servidor de Micro Base de Datos"
	@echo "Autor: Agustin"
	@echo "Materia: Sistemas Operativos - UNLAM"
	@echo ""
	@echo "Directorios:"
	@echo "  Fuentes:    $(SRCDIR)/"
	@echo "  Binarios:   $(BINDIR)/"
	@echo ""
	@echo "Archivos:"
	@echo "  Servidor:   $(SERVER_SRC) → $(SERVER_BIN)"
	@echo "  Cliente:    $(CLIENT_SRC) → $(CLIENT_BIN)"
	@echo ""
	@echo "Compilador: $(CC) $(CFLAGS)"
	@echo ""

# Ayuda detallada
help:
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  Makefile - Sistema Cliente-Servidor de Base de Datos"
	@echo "════════════════════════════════════════════════════════════════"
	@echo ""
	@echo "📦 COMPILACIÓN:"
	@echo "  make              - Compilar servidor y cliente"
	@echo "  make all          - Compilar servidor y cliente (alias)"
	@echo "  make server       - Compilar solo el servidor"
	@echo "  make client       - Compilar solo el cliente"
	@echo "  make rebuild      - Limpiar y recompilar todo"
	@echo "  make clean        - Eliminar archivos compilados"
	@echo "  make distclean    - Eliminar todo (compilados + temporales)"
	@echo ""
	@echo "▶️  EJECUCIÓN:"
	@echo "  make run-server   - Compilar y ejecutar servidor"
	@echo "  make run-client   - Compilar y ejecutar cliente"
	@echo ""
	@echo "⚙️  PARÁMETROS CONFIGURABLES:"
	@echo "  PORT=<puerto>           - Puerto del servidor (default: 8080)"
	@echo "  MAX_CLIENTS=<num>       - Máximo de clientes (default: 5)"
	@echo "  MAX_QUEUE=<num>         - Máximo en cola (default: 10)"
	@echo "  SERVER_HOST=<host>      - Host del servidor (default: 127.0.0.1)"
	@echo ""
	@echo "📝 EJEMPLOS CON PARÁMETROS:"
	@echo "  make run-server PORT=4321 MAX_CLIENTS=10 MAX_QUEUE=20"
	@echo "  make run-client SERVER_HOST=192.168.1.100 PORT=4321"
	@echo "  make run-client SERVER_HOST=miservidor.com PORT=9000"
	@echo ""
	@echo "⚡ UTILIDADES:"
	@echo "  make info         - Mostrar información del proyecto"
	@echo "  make help         - Mostrar esta ayuda"
	@echo ""
	@echo "📖 USO MANUAL:"
	@echo "  $(SERVER_BIN) -p <puerto> [-n <max_clientes>] [-m <max_cola>]"
	@echo "  $(CLIENT_BIN) -s <servidor> -p <puerto>"
	@echo ""
	@echo "  Ejemplos:"
	@echo "    $(SERVER_BIN) -p 8080 -n 10 -m 20"
	@echo "    $(CLIENT_BIN) -s 127.0.0.1 -p 8080"
	@echo ""
	@echo "📚 DOCUMENTACIÓN:"
	@echo "  README.md                 - Manual completo"
	@echo "  ARCHITECTURE.md           - Documentación técnica"
	@echo "  QUICKSTART.md             - Guía rápida"
	@echo "  RESUMEN.md                - Resumen ejecutivo"
	@echo "  TESTING.md                - Guía de testing"
	@echo "  INSTRUCCIONES_ENTREGA.md  - Para el evaluador"
	@echo ""
	@echo "════════════════════════════════════════════════════════════════"

# Alias
server: $(SERVER_BIN)
client: $(CLIENT_BIN)

.PHONY: all clean distclean run-server run-client rebuild info help server client
