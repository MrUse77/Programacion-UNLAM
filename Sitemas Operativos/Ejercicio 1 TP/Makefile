# Makefile para el sistema de generación distribuida de datos CSV con IPC

# Directorios
OBJDIR = obj
SRCDIR = src
INCDIR = $(SRCDIR)/includes
BINDIR = bin

# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I$(INCDIR)
LDFLAGS = 

# Archivos fuente
SOURCES = main.c coordinator.c generators.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
HEADERS = $(INCDIR)/variables.h $(INCDIR)/coordinator.h $(INCDIR)/generator.h

# Ejecutable
TARGET = $(BINDIR)/DataGenerator

# Parámetros por defecto para ejecución
REGISTERS = 1000
CHILDS = 4
DIR_ARCHIVO_SALIDA = salida.csv

# Regla por defecto
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS) | $(BINDIR)
	@echo "Enlazando ejecutable..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Ejecutable creado: $(TARGET)"
	@echo ""

# Regla patrón: compilar cada .c en obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorio obj si no existe
$(OBJDIR):
	@echo "Creando directorio $(OBJDIR)..."
	@mkdir -p $(OBJDIR)

# Crear directorio bin si no existe
$(BINDIR):
	@echo "Creando directorio $(BINDIR)..."
	@mkdir -p $(BINDIR)

# Limpiar archivos compilados
clean:
	@echo "Limpiando archivos compilados..."
	@rm -rf $(OBJDIR) $(TARGET)
	@echo "Limpieza completada"

# Limpiar todo incluyendo archivos generados
distclean: clean
	@echo "Limpieza completa..."
	@rm -f generated_data.csv
	@rm -f *~
	@rm -f $(SRCDIR)/*~
	@rm -f $(INCDIR)/*~
	@rm -rf $(BINDIR)
	@echo "Limpieza completa finalizada"

# Compilar y ejecutar con parámetros por defecto
init: $(TARGET)
	@echo "Ejecutando con $(CHILDS) generadores y $(REGISTERS) registros..."
	@echo ""
	$(TARGET) $(CHILDS) $(REGISTERS)

# Ejecutar con parámetros personalizados
run: $(TARGET)
	@echo "Uso: make run CHILDS=<num> REGISTERS=<num> DIR_ARCHIVO_SALIDA=<dir>"
	@echo "Ejemplo: make run CHILDS=8 REGISTERS=5000 DIR_ARCHIVO_SALIDA=salida"
	@echo ""
	@echo "Ejecutando con $(CHILDS) generadores y $(REGISTERS) registros..."
	@echo ""
	$(TARGET) $(CHILDS) $(REGISTERS) $(DIR_ARCHIVO_SALIDA)
	@echo ""

# Test rápido con configuración mínima
test-small: $(TARGET)
	@echo "Test pequeño: 2 generadores, 100 registros"
	@echo ""
	$(TARGET) 2 100
	@echo ""
	@echo "Verificando resultado..."
	@wc -l generated_data.csv
	@echo ""

# Test con configuración media
test-medium: $(TARGET)
	@echo "Test medio: 4 generadores, 1000 registros"
	@echo ""
	$(TARGET) 4 1000
	@echo ""
	@echo "Verificando resultado..."
	@wc -l generated_data.csv
	@head -5 generated_data.csv
	@echo ""

# Test con configuración grande
test-large: $(TARGET)
	@echo "Test grande: 10 generadores, 10000 registros"
	@echo ""
	$(TARGET) 10 10000
	@echo ""
	@echo "Verificando resultado..."
	@wc -l generated_data.csv
	@echo ""

# Test de alta concurrencia
test-concurrent: $(TARGET)
	@echo "Test de alta concurrencia: 20 generadores, 50000 registros"
	@echo ""
	$(TARGET) 20 50000
	@echo ""
	@echo "Verificando resultado..."
	@wc -l generated_data.csv
	@echo ""

# Verificar integridad de datos generados
verify:
	@echo "=== Verificación de Integridad de Datos ==="
	@echo ""
	@if [ ! -f generated_data.csv ]; then \
		echo "❌ Error: No se encontró generated_data.csv"; \
		echo "   Ejecuta 'make init' primero"; \
		exit 1; \
	fi
	@echo "1. Verificando archivo..."
	@wc -l generated_data.csv
	@echo ""
	@echo "2. Verificando header..."
	@head -1 generated_data.csv
	@echo ""
	@echo "3. Buscando IDs duplicados..."
	@DUPES=$$(cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l); \
	if [ $$DUPES -eq 0 ]; then \
		echo "✅ Sin IDs duplicados"; \
	else \
		echo "❌ $$DUPES IDs duplicados encontrados"; \
	fi
	@echo ""
	@echo "4. Verificando formato de líneas..."
	@INVALID=$$(awk -F',' 'NR>1 {if(NF!=7) print NR}' generated_data.csv | wc -l); \
	if [ $$INVALID -eq 0 ]; then \
		echo "✅ Todas las líneas tienen 7 campos"; \
	else \
		echo "❌ $$INVALID líneas con formato inválido"; \
	fi
	@echo ""
	@echo "5. Muestra de datos:"
	@head -10 generated_data.csv
	@echo ""

# Mostrar estadísticas de datos generados
stats:
	@echo "=== Estadísticas de Datos Generados ==="
	@echo ""
	@if [ ! -f generated_data.csv ]; then \
		echo "❌ Error: No se encontró generated_data.csv"; \
		echo "   Ejecuta 'make init' primero"; \
		exit 1; \
	fi
	@echo "Total de registros:"
	@tail -n +2 generated_data.csv | wc -l
	@echo ""
	@echo "Distribución por Departamento:"
	@cut -d',' -f5 generated_data.csv | tail -n +2 | sort | uniq -c | sort -rn
	@echo ""
	@echo "Edad promedio:"
	@awk -F',' 'NR>1 {sum+=$$3; count++} END {printf "  %.1f años\n", sum/count}' generated_data.csv
	@echo ""
	@echo "Salario promedio:"
	@awk -F',' 'NR>1 {sum+=$$6; count++} END {printf "  $%.2f\n", sum/count}' generated_data.csv
	@echo ""

# Verificar recursos IPC
check-ipc:
	@echo "=== Estado de Recursos IPC ==="
	@echo ""
	@echo "Memoria compartida:"
	@ipcs -m | grep $$(whoami) || echo "  No hay segmentos de memoria compartida"
	@echo ""
	@echo "Semáforos:"
	@ipcs -s | grep $$(whoami) || echo "  No hay conjuntos de semáforos"
	@echo ""

# Limpiar recursos IPC manualmente (usar solo si es necesario)
clean-ipc:
	@echo "⚠️  Limpiando recursos IPC del usuario..."
	@echo "Esto eliminará TODOS los recursos IPC de tu usuario"
	@read -p "¿Estás seguro? [y/N] " confirm; \
	if [ "$$confirm" = "y" ] || [ "$$confirm" = "Y" ]; then \
		ipcrm -a 2>/dev/null || echo "No hay recursos IPC para limpiar"; \
	else \
		echo "Operación cancelada"; \
	fi

# Benchmark de rendimiento
benchmark: $(TARGET)
	@echo "=== Benchmark de Rendimiento ==="
	@echo ""
	@for GENS in 1 2 4 8 10; do \
		echo "Testing con $$GENS generadores..."; \
		time $(TARGET) $$GENS 5000 2>&1 | grep -E "real|Sistema finalizado"; \
		rm -f generated_data.csv; \
		echo ""; \
	done

# Recompilar completamente
rebuild: clean all
	@echo "Recompilación completa finalizada"

# Mostrar información del proyecto
info:
	@echo "=== Información del Proyecto ==="
	@echo ""
	@echo "Proyecto: Sistema de Generación Distribuida de Datos CSV con IPC"
	@echo "Autor: Agustin"
	@echo "Materia: Sistemas Operativos - UNLAM"
	@echo ""
	@echo "Directorios:"
	@echo "  Fuentes:    $(SRCDIR)/"
	@echo "  Headers:    $(INCDIR)/"
	@echo "  Objetos:    $(OBJDIR)/"
	@echo "  Binarios:   $(BINDIR)/"
	@echo ""
	@echo "Archivos:"
	@echo "  Fuentes:    $(SOURCES)"
	@echo "  Headers:    $(notdir $(HEADERS))"
	@echo "  Ejecutable: $(TARGET)"
	@echo ""
	@echo "Compilador: $(CC) $(CFLAGS)"
	@echo ""

# Ayuda detallada
help:
	@echo "════════════════════════════════════════════════════════════════"
	@echo "  Makefile - Sistema de Generación Distribuida de Datos CSV"
	@echo "════════════════════════════════════════════════════════════════"
	@echo ""
	@echo "📦 COMPILACIÓN:"
	@echo "  make              - Compilar el proyecto"
	@echo "  make all          - Compilar el proyecto (alias)"
	@echo "  make rebuild      - Limpiar y recompilar"
	@echo "  make clean        - Eliminar archivos compilados"
	@echo "  make distclean    - Eliminar todo (compilados + generados)"
	@echo ""
	@echo "▶️  EJECUCIÓN:"
	@echo "  make init         - Compilar y ejecutar (4 gen, 1000 reg)"
	@echo "  make run          - Ejecutar con parámetros personalizados"
	@echo "                      Ejemplo: make run CHILDS=8 REGISTERS=5000"
	@echo ""
	@echo "🧪 TESTING:"
	@echo "  make test-small   - Test pequeño (2 gen, 100 reg)"
	@echo "  make test-medium  - Test medio (4 gen, 1000 reg)"
	@echo "  make test-large   - Test grande (10 gen, 10000 reg)"
	@echo "  make test-concurrent - Test concurrencia (20 gen, 50000 reg)"
	@echo ""
	@echo "✅ VERIFICACIÓN:"
	@echo "  make verify       - Verificar integridad de datos generados"
	@echo "  make stats        - Mostrar estadísticas de datos"
	@echo "  make check-ipc    - Verificar recursos IPC activos"
	@echo ""
	@echo "⚡ UTILIDADES:"
	@echo "  make benchmark    - Ejecutar benchmark de rendimiento"
	@echo "  make clean-ipc    - Limpiar recursos IPC manualmente"
	@echo "  make info         - Mostrar información del proyecto"
	@echo "  make help         - Mostrar esta ayuda"
	@echo ""
	@echo "📖 USO MANUAL:"
	@echo "  $(TARGET) <num_generadores> <total_registros>"
	@echo ""
	@echo "  Parámetros:"
	@echo "    num_generadores: 1-20"
	@echo "    total_registros: 1-100000"
	@echo ""
	@echo "  Ejemplo:"
	@echo "    $(TARGET) 8 5000"
	@echo ""
	@echo "📚 DOCUMENTACIÓN:"
	@echo "  README.md             - Manual completo"
	@echo "  ARCHITECTURE.md       - Documentación técnica"
	@echo "  QUICKSTART.md         - Guía rápida"
	@echo "  RESUMEN.md            - Resumen ejecutivo"
	@echo "  TESTING.md            - Guía de testing"
	@echo "  INSTRUCCIONES_ENTREGA.md - Para el evaluador"
	@echo ""
	@echo "════════════════════════════════════════════════════════════════"

.PHONY: all clean distclean init run test-small test-medium test-large test-concurrent \
        verify stats check-ipc clean-ipc benchmark rebuild info help
