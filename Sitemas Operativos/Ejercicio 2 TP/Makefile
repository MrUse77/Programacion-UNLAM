# Makefile para el sistema cliente-servidor de base de datos

CC = gcc
CFLAGS = -Wall -Wextra -pthread -g
LDFLAGS = -pthread

# Archivos fuente
SERVER_SRC = server.c
CLIENT_SRC = client.c

# Ejecutables
SERVER_BIN = server
CLIENT_BIN = client

# Regla por defecto
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compilar servidor
$(SERVER_BIN): $(SERVER_SRC)
	@echo "Compilando servidor..."
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_SRC) $(LDFLAGS)
	@echo "Servidor compilado: $(SERVER_BIN)"

# Compilar cliente
$(CLIENT_BIN): $(CLIENT_SRC)
	@echo "Compilando cliente..."
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC) $(LDFLAGS)
	@echo "Cliente compilado: $(CLIENT_BIN)"

# Limpiar archivos compilados y temporales
clean:
	@echo "Limpiando archivos compilados..."
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
	rm -f *.o
	rm -f database.tmp
	rm -f *.log
	@echo "Limpieza completada"

# Limpiar todo incluyendo backups
distclean: clean
	@echo "Limpieza completa..."
	rm -f *~
	rm -f database.csv.bak

# Ejecutar servidor con parámetros por defecto
run-server: $(SERVER_BIN)
	@echo "Iniciando servidor en puerto 8080..."
	./$(SERVER_BIN) -p 8080 -n 5 -m 10

# Ejecutar cliente conectándose a localhost
run-client: $(CLIENT_BIN)
	@echo "Conectando cliente a localhost:8080..."
	./$(CLIENT_BIN) -s 127.0.0.1 -p 8080

# Ayuda
help:
	@echo "Makefile para sistema cliente-servidor de base de datos"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make              - Compilar servidor y cliente"
	@echo "  make server       - Compilar solo el servidor"
	@echo "  make client       - Compilar solo el cliente"
	@echo "  make clean        - Eliminar archivos compilados"
	@echo "  make distclean    - Eliminar todos los archivos generados"
	@echo "  make run-server   - Compilar y ejecutar servidor (puerto 8080)"
	@echo "  make run-client   - Compilar y ejecutar cliente (localhost:8080)"
	@echo "  make help         - Mostrar esta ayuda"
	@echo ""
	@echo "Uso manual:"
	@echo "  Servidor: ./server -p <puerto> [-n <max_clientes>] [-m <max_cola>]"
	@echo "  Cliente:  ./client -s <servidor> -p <puerto>"

# Alias
server: $(SERVER_BIN)
client: $(CLIENT_BIN)

.PHONY: all clean distclean run-server run-client help server client
