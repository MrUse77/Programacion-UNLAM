# Makefile para Sistema Cliente-Servidor de Micro Base de Datos
# Compilador y opciones

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2 -D_GNU_SOURCE
LDFLAGS = 

# Bibliotecas
SERVER_LIBS = -lpthread
CLIENT_LIBS = -lreadline -lhistory

# Directorios
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Archivos fuente
SERVER_SOURCES = src/server.c src/database/protocol.c src/database/database.c
SERVER_COMMON_SOURCES = server.c protocol.c database.c
CLIENT_COMMON_SOURCES = client.c protocol.c
CLIENT_SOURCES = src/client.c database/protocol.c
COMMON_SOURCES = src/database/protocol.c

# Archivos objeto
SERVER_OBJECTS = $(addprefix $(OBJ_DIR)/, $(SERVER_COMMON_SOURCES:.c=.o))
CLIENT_OBJECTS = $(addprefix $(OBJ_DIR)/, $(CLIENT_COMMON_SOURCES:.c=.o))

# Ejecutables
SERVER_TARGET = $(BIN_DIR)/microdb_server
CLIENT_TARGET = $(BIN_DIR)/microdb_client

# Headers
HEADERS = src/includes/microdb.h src/includes/protocol.h src/includes/database.h

# Targets principales
.PHONY: all clean install uninstall help test

all: directories $(SERVER_TARGET) $(CLIENT_TARGET)

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Compilar servidor
$(SERVER_TARGET): $(SERVER_OBJECTS)
	@echo "Enlazando servidor..."
	$(CC) $(SERVER_OBJECTS) -o $@ $(SERVER_LIBS) $(LDFLAGS)
	@echo "Servidor compilado: $@"

# Compilar cliente
$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	@echo "Enlazando cliente..."
	$(CC) $(CLIENT_OBJECTS) -o $@ $(CLIENT_LIBS) $(LDFLAGS)
	@echo "Cliente compilado: $@"

# Regla para archivos objeto del servidor
$(OBJ_DIR)/server.o: src/server.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/database.o: src/database/database.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para archivos objeto del cliente
$(OBJ_DIR)/client.o: src/client.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para protocol.o (compartido)
$(OBJ_DIR)/protocol.o: src/database/protocol.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f *.o *.core core
	rm -f database_backup.csv
	@echo "Limpieza completada"

# Compilación en modo debug
debug: CFLAGS += -DDEBUG -g3 -O0
debug: all

# Compilación en modo release
release: CFLAGS += -DNDEBUG -O3
release: clean all

# Test de compilación y verificación básica
test: all
	@echo "Ejecutando tests básicos..."
	@echo "1. Verificando que los ejecutables existen..."
	@test -f $(SERVER_TARGET) && echo "  ✓ Servidor compilado correctamente" || echo "  ✗ Error: servidor no encontrado"
	@test -f $(CLIENT_TARGET) && echo "  ✓ Cliente compilado correctamente" || echo "  ✗ Error: cliente no encontrado"
	@echo "2. Verificando permisos de ejecución..."
	@test -x $(SERVER_TARGET) && echo "  ✓ Servidor es ejecutable" || echo "  ✗ Error: servidor no ejecutable"
	@test -x $(CLIENT_TARGET) && echo "  ✓ Cliente es ejecutable" || echo "  ✗ Error: cliente no ejecutable"
	@echo "3. Verificando archivos de configuración..."
	@test -f config.txt && echo "  ✓ Archivo de configuración encontrado" || echo "  ✗ Error: config.txt no encontrado"
	@test -f database.csv && echo "  ✓ Base de datos encontrada" || echo "  ✗ Error: database.csv no encontrado"
	@echo "Tests básicos completados"

# Crear backup de la base de datos
backup:
	@echo "Creando backup de la base de datos..."
	@if [ -f database.csv ]; then \
		cp database.csv database_backup_$(shell date +%Y%m%d_%H%M%S).csv; \
		echo "Backup creado: database_backup_$(shell date +%Y%m%d_%H%M%S).csv"; \
	else \
		echo "Error: database.csv no encontrado"; \
	fi

# Restaurar base de datos desde backup
restore:
	@echo "Restaurando base de datos desde backup..."
	@if [ -f database_backup.csv ]; then \
		cp database_backup.csv database.csv; \
		echo "Base de datos restaurada desde database_backup.csv"; \
	else \
		echo "Error: database_backup.csv no encontrado"; \
	fi

# Mostrar información del sistema
info:
	@echo "=== Información del Sistema Micro Base de Datos ==="
	@echo "Compilador: $(CC)"
	@echo "Flags de compilación: $(CFLAGS)"
	@echo "Bibliotecas del servidor: $(SERVER_LIBS)"
	@echo "Bibliotecas del cliente: $(CLIENT_LIBS)"
	@echo "Archivos fuente del servidor: $(SERVER_SOURCES)"
	@echo "Archivos fuente del cliente: $(CLIENT_SOURCES)"
	@echo "Ejecutable del servidor: $(SERVER_TARGET)"
	@echo "Ejecutable del cliente: $(CLIENT_TARGET)"
	@echo "================================================="

# Ejecutar servidor en modo desarrollo
run-server: $(SERVER_TARGET)
	@echo "Iniciando servidor en modo desarrollo..."
	./$(SERVER_TARGET) config.txt

# Ejecutar cliente de prueba
run-client: $(CLIENT_TARGET)
	@echo "Iniciando cliente..."
	./$(CLIENT_TARGET)

# Verificar la sintaxis de los archivos C
check-syntax:
	@echo "Verificando sintaxis de archivos fuente..."
	@for file in *.c; do \
		echo "  Verificando $$file..."; \
		$(CC) $(CFLAGS) -fsyntax-only $$file && echo "    ✓ Sintaxis correcta" || echo "    ✗ Errores de sintaxis"; \
	done

# Generar documentación con doxygen (si está disponible)
docs:
	@if which doxygen > /dev/null; then \
		echo "Generando documentación..."; \
		doxygen Doxyfile 2>/dev/null || echo "Archivo Doxyfile no encontrado"; \
	else \
		echo "Doxygen no está instalado"; \
	fi

# Mostrar ayuda
help:
	@echo "=== Sistema Cliente-Servidor de Micro Base de Datos ==="
	@echo ""
	@echo "Targets disponibles:"
	@echo "  all          - Compilar servidor y cliente (target por defecto)"
	@echo "  clean        - Limpiar archivos generados"
	@echo "  debug        - Compilar en modo debug"
	@echo "  release      - Compilar en modo release optimizado"
	@echo "  test         - Ejecutar tests básicos"
	@echo "  backup       - Crear backup de la base de datos"
	@echo "  restore      - Restaurar base de datos desde backup"
	@echo "  info         - Mostrar información de configuración"
	@echo "  run-server   - Ejecutar servidor en modo desarrollo"
	@echo "  run-client   - Ejecutar cliente de prueba"
	@echo "  check-syntax - Verificar sintaxis de archivos fuente"
	@echo "  docs         - Generar documentación"
	@echo "  help         - Mostrar esta ayuda"
	@echo ""
	@echo "Ejemplos de uso:"
	@echo "  make                    # Compilar todo"
	@echo "  make clean all          # Limpiar y compilar"
	@echo "  make install            # Instalar sistema"
	@echo "  make run-server         # Ejecutar servidor"
	@echo "  make run-client         # Ejecutar cliente"
	@echo ""
	@echo "======================================================="

# Dependencias implícitas
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@
