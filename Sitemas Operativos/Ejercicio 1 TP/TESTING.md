# Guía de Testing y Monitoreo

## Tabla de Contenidos

- [Tests Básicos](#tests-básicos)
- [Tests de Validación](#tests-de-validación)
- [Tests de Concurrencia](#tests-de-concurrencia)
- [Tests de Señales](#tests-de-señales)
- [Monitoreo del Sistema](#monitoreo-del-sistema)
- [Verificación de Integridad](#verificación-de-integridad)
- [Tests de Rendimiento](#tests-de-rendimiento)
- [Debugging](#debugging)

## Tests Básicos

### Test 1: Compilación

```bash
# Compilar desde cero
make clean
make

# Verificar que se creó el ejecutable
ls -lh bin/DataGenerator

# Verificar permisos de ejecución
file bin/DataGenerator
```

**Resultado esperado**:

```
bin/DataGenerator: ELF 64-bit LSB executable
```

### Test 2: Ayuda y Parámetros

```bash
# Sin parámetros (debe mostrar ayuda)
./bin/DataGenerator

# Resultado esperado:
# Uso: ./bin/DataGenerator <num_generadores> <total_registros>
```

### Test 3: Ejecución Mínima

```bash
# Configuración mínima: 1 generador, 1 registro
./bin/DataGenerator 1 1

# Verificar resultado
cat generated_data.csv
```

**Resultado esperado**:

```csv
ID,Name,Age,City,Department,Salary,Experience
1,Juan,45,Madrid,IT,87543,15
```

### Test 4: Ejecución Estándar

```bash
# Configuración típica
./bin/DataGenerator 4 1000

# Verificar cantidad de registros
wc -l generated_data.csv
# Debe mostrar: 1001 (1 header + 1000 datos)
```

## Tests de Validación

### Test 5: Validación de Parámetros

```bash
# Test con 0 generadores (inválido)
./bin/DataGenerator 0 1000
# Error esperado: Número de generadores debe estar entre 1 y 20

# Test con generadores negativos
./bin/DataGenerator -5 1000
# Error esperado: Número de generadores debe estar entre 1 y 20

# Test con demasiados generadores
./bin/DataGenerator 25 1000
# Error esperado: Número de generadores debe estar entre 1 y 20

# Test con 0 registros
./bin/DataGenerator 4 0
# Error esperado: Total de registros debe estar entre 1 y 100000

# Test con demasiados registros
./bin/DataGenerator 4 150000
# Error esperado: Total de registros debe estar entre 1 y 100000
```

### Test 6: Verificación de IDs Únicos

```bash
# Generar datos
./bin/DataGenerator 10 5000

# Extraer IDs y buscar duplicados
cut -d',' -f1 generated_data.csv | tail -n +2 | sort -n | uniq -d

# No debe mostrar nada (sin duplicados)

# Verificar rango de IDs
cut -d',' -f1 generated_data.csv | tail -n +2 | sort -n | head -1
# Debe mostrar: 1

cut -d',' -f1 generated_data.csv | tail -n +2 | sort -n | tail -1
# Debe mostrar: 5000
```

### Test 7: Verificación de Conteo

```bash
# Generar datos
./bin/DataGenerator 5 3000

# Contar registros (excluyendo header)
REGISTROS=$(tail -n +2 generated_data.csv | wc -l)
echo "Registros generados: $REGISTROS"

# Debe ser exactamente 3000
if [ $REGISTROS -eq 3000 ]; then
    echo "✅ Test PASADO: Conteo correcto"
else
    echo "❌ Test FALLIDO: Se esperaban 3000, se obtuvieron $REGISTROS"
fi
```

### Test 8: Validación de Formato CSV

```bash
# Generar datos
./bin/DataGenerator 4 100

# Verificar header
head -1 generated_data.csv
# Debe mostrar: ID,Name,Age,City,Department,Salary,Experience

# Verificar número de campos por línea
awk -F',' 'NR>1 {if(NF!=7) print "Línea " NR ": " NF " campos"}' generated_data.csv
# No debe mostrar nada (todas las líneas deben tener 7 campos)

# Verificar que no hay líneas vacías
grep -c '^$' generated_data.csv
# Debe mostrar: 0
```

## Tests de Concurrencia

### Test 9: Múltiples Generadores

```bash
# Test con diferentes cantidades de generadores
for N in 1 2 4 8 10 15 20; do
    echo "=== Testing con $N generadores ==="
    ./bin/DataGenerator $N 1000

    # Verificar conteo
    COUNT=$(tail -n +2 generated_data.csv | wc -l)
    if [ $COUNT -eq 1000 ]; then
        echo "✅ $N generadores: OK"
    else
        echo "❌ $N generadores: FALLO (generados: $COUNT)"
    fi

    # Verificar IDs únicos
    DUPES=$(cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l)
    if [ $DUPES -eq 0 ]; then
        echo "✅ $N generadores: Sin duplicados"
    else
        echo "❌ $N generadores: $DUPES IDs duplicados"
    fi

    rm -f generated_data.csv
    echo ""
done
```

### Test 10: Alta Concurrencia

```bash
# Test con máxima concurrencia y muchos registros
./bin/DataGenerator 20 50000

# Verificar integridad
echo "Verificando integridad..."
TOTAL=$(tail -n +2 generated_data.csv | wc -l)
DUPES=$(cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l)

echo "Total registros: $TOTAL"
echo "IDs duplicados: $DUPES"

if [ $TOTAL -eq 50000 ] && [ $DUPES -eq 0 ]; then
    echo "✅ Test de alta concurrencia PASADO"
else
    echo "❌ Test de alta concurrencia FALLIDO"
fi
```

### Test 11: Race Conditions

```bash
# Ejecutar múltiples veces para detectar race conditions
for i in {1..10}; do
    echo "Iteración $i de 10"
    ./bin/DataGenerator 10 5000 > /dev/null 2>&1

    # Verificar IDs únicos
    DUPES=$(cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l)
    if [ $DUPES -ne 0 ]; then
        echo "❌ Race condition detectada en iteración $i"
        exit 1
    fi
    rm -f generated_data.csv
done
echo "✅ No se detectaron race conditions en 10 iteraciones"
```

## Tests de Señales

### Test 12: Interrupción con SIGINT (Ctrl+C)

```bash
# Terminal 1: Iniciar generación grande
./bin/DataGenerator 10 100000

# Inmediatamente presionar Ctrl+C

# Verificar que no quedan recursos IPC
ipcs -m | grep $(whoami)
ipcs -s | grep $(whoami)

# No debe haber segmentos huérfanos
```

### Test 13: Terminación con SIGTERM

```bash
# Terminal 1: Iniciar programa
./bin/DataGenerator 10 50000 &
PID=$!

# Terminal 2: Enviar SIGTERM
sleep 2
kill -TERM $PID

# Esperar terminación
wait $PID

# Verificar cleanup
ipcs -m -s
# No debe haber recursos del programa
```

### Test 14: Kill Abrupto (SIGKILL)

```bash
# Iniciar programa
./bin/DataGenerator 10 50000 &
PID=$!

# Kill inmediato (simula crash)
sleep 1
kill -9 $PID

# Verificar recursos IPC
echo "Verificando recursos IPC huérfanos..."
ipcs -m | grep $(whoami)
ipcs -s | grep $(whoami)

# Limpiar manualmente si es necesario
# ipcrm -a
```

## Monitoreo del Sistema

### Test 15: Monitoreo de Procesos

```bash
# Terminal 1: Ejecutar programa
./bin/DataGenerator 8 50000

# Terminal 2: Monitorear procesos en tiempo real
watch -n 1 'ps aux | grep DataGenerator | grep -v grep'

# Debe mostrar:
# - 1 proceso principal
# - 8 procesos generadores
# - Total: 9 procesos
```

### Test 16: Árbol de Procesos

```bash
# Iniciar programa
./bin/DataGenerator 5 10000 &
PID=$!

# Ver árbol de procesos
pstree -p $PID

# Debe mostrar:
# DataGenerator(PID)─┬─DataGenerator(PID1)
#                    ├─DataGenerator(PID2)
#                    ├─DataGenerator(PID3)
#                    ├─DataGenerator(PID4)
#                    └─DataGenerator(PID5)
```

### Test 17: Monitoreo de IPC

```bash
# Terminal 1: Ejecutar programa
./bin/DataGenerator 5 20000

# Terminal 2: Monitorear recursos IPC
watch -n 1 'ipcs -m -s'

# Debe mostrar:
# - 1 segmento de memoria compartida
# - 1 conjunto de semáforos con 3 semáforos
```

### Test 18: Monitoreo de Archivo

```bash
# Terminal 1: Ejecutar programa
./bin/DataGenerator 10 50000

# Terminal 2: Ver archivo creciendo
watch -n 1 'wc -l generated_data.csv'

# Terminal 3: Ver tamaño del archivo
watch -n 1 'du -h generated_data.csv'
```

### Test 19: Uso de CPU

```bash
# Ejecutar programa
./bin/DataGenerator 10 100000 &
PID=$!

# Monitorear uso de CPU
top -p $PID

# O usar htop si está disponible
htop -p $PID

# Presionar 't' para ver árbol de threads/procesos
```

## Verificación de Integridad

### Test 20: Integridad de Datos

```bash
# Generar datos
./bin/DataGenerator 10 10000

# Script de verificación completa
cat > verify_data.sh << 'EOF'
#!/bin/bash

FILE="generated_data.csv"

echo "=== Verificación de Integridad de Datos ==="

# 1. Verificar que existe el archivo
if [ ! -f "$FILE" ]; then
    echo "❌ Archivo no encontrado"
    exit 1
fi
echo "✅ Archivo existe"

# 2. Verificar header
HEADER=$(head -1 "$FILE")
EXPECTED="ID,Name,Age,City,Department,Salary,Experience"
if [ "$HEADER" = "$EXPECTED" ]; then
    echo "✅ Header correcto"
else
    echo "❌ Header incorrecto: $HEADER"
fi

# 3. Contar registros
TOTAL=$(tail -n +2 "$FILE" | wc -l)
echo "📊 Total de registros: $TOTAL"

# 4. Verificar IDs únicos
DUPES=$(cut -d',' -f1 "$FILE" | tail -n +2 | sort | uniq -d | wc -l)
if [ $DUPES -eq 0 ]; then
    echo "✅ Sin IDs duplicados"
else
    echo "❌ $DUPES IDs duplicados encontrados"
fi

# 5. Verificar rango de IDs
MIN_ID=$(cut -d',' -f1 "$FILE" | tail -n +2 | sort -n | head -1)
MAX_ID=$(cut -d',' -f1 "$FILE" | tail -n +2 | sort -n | tail -1)
echo "📊 Rango de IDs: $MIN_ID - $MAX_ID"

# 6. Verificar formato de cada línea
INVALID=$(awk -F',' 'NR>1 {if(NF!=7) print NR}' "$FILE" | wc -l)
if [ $INVALID -eq 0 ]; then
    echo "✅ Todas las líneas tienen 7 campos"
else
    echo "❌ $INVALID líneas con formato inválido"
fi

# 7. Verificar rangos de datos
echo "📊 Validando rangos de datos..."

# Edad: 18-65
INVALID_AGE=$(awk -F',' 'NR>1 {if($3<18 || $3>65) print NR}' "$FILE" | wc -l)
if [ $INVALID_AGE -eq 0 ]; then
    echo "✅ Todas las edades en rango [18-65]"
else
    echo "❌ $INVALID_AGE edades fuera de rango"
fi

# Salario: 30000-100000
INVALID_SALARY=$(awk -F',' 'NR>1 {if($6<30000 || $6>100000) print NR}' "$FILE" | wc -l)
if [ $INVALID_SALARY -eq 0 ]; then
    echo "✅ Todos los salarios en rango [30000-100000]"
else
    echo "❌ $INVALID_SALARY salarios fuera de rango"
fi

# Experiencia: 0 <= exp <= (edad-18)
INVALID_EXP=$(awk -F',' 'NR>1 {if($7<0 || $7>($3-18)) print NR}' "$FILE" | wc -l)
if [ $INVALID_EXP -eq 0 ]; then
    echo "✅ Experiencia válida respecto a edad"
else
    echo "❌ $INVALID_EXP registros con experiencia inválida"
fi

echo ""
echo "=== Verificación Completada ==="
EOF

chmod +x verify_data.sh
./verify_data.sh
```

### Test 21: Estadísticas de Datos

```bash
# Generar datos
./bin/DataGenerator 8 10000

# Análisis estadístico
cat > stats.sh << 'EOF'
#!/bin/bash

FILE="generated_data.csv"

echo "=== Estadísticas de Datos Generados ==="
echo ""

# Distribución por nombre
echo "📊 Distribución por Nombre:"
cut -d',' -f2 "$FILE" | tail -n +2 | sort | uniq -c | sort -rn

echo ""

# Distribución por ciudad
echo "📊 Distribución por Ciudad:"
cut -d',' -f4 "$FILE" | tail -n +2 | sort | uniq -c | sort -rn

echo ""

# Distribución por departamento
echo "📊 Distribución por Departamento:"
cut -d',' -f5 "$FILE" | tail -n +2 | sort | uniq -c | sort -rn

echo ""

# Estadísticas de edad
echo "📊 Estadísticas de Edad:"
awk -F',' 'NR>1 {sum+=$3; count++} END {print "  Promedio: " sum/count}' "$FILE"
awk -F',' 'NR>1 {print $3}' "$FILE" | sort -n | head -1 | xargs -I {} echo "  Mínima: {}"
awk -F',' 'NR>1 {print $3}' "$FILE" | sort -n | tail -1 | xargs -I {} echo "  Máxima: {}"

echo ""

# Estadísticas de salario
echo "📊 Estadísticas de Salario:"
awk -F',' 'NR>1 {sum+=$6; count++} END {print "  Promedio: $" sum/count}' "$FILE"
awk -F',' 'NR>1 {print $6}' "$FILE" | sort -n | head -1 | xargs -I {} echo "  Mínimo: ${}"
awk -F',' 'NR>1 {print $6}' "$FILE" | sort -n | tail -1 | xargs -I {} echo "  Máximo: ${}"

echo ""

# Estadísticas de experiencia
echo "📊 Estadísticas de Experiencia:"
awk -F',' 'NR>1 {sum+=$7; count++} END {print "  Promedio: " sum/count " años"}' "$FILE"
awk -F',' 'NR>1 {print $7}' "$FILE" | sort -n | head -1 | xargs -I {} echo "  Mínima: {} años"
awk -F',' 'NR>1 {print $7}' "$FILE" | sort -n | tail -1 | xargs -I {} echo "  Máxima: {} años"
EOF

chmod +x stats.sh
./stats.sh
```

## Tests de Rendimiento

### Test 22: Benchmark de Throughput

```bash
# Script de benchmark
cat > benchmark.sh << 'EOF'
#!/bin/bash

echo "=== Benchmark de Rendimiento ==="
echo ""

RECORDS=10000

for GENS in 1 2 4 8 10 15 20; do
    echo -n "Generadores: $GENS - "

    # Medir tiempo de ejecución
    TIME=$( { time ./bin/DataGenerator $GENS $RECORDS > /dev/null 2>&1; } 2>&1 | grep real | awk '{print $2}' )

    # Calcular throughput (registros por segundo)
    # Convertir tiempo a segundos
    SECONDS=$(echo $TIME | awk -F'[ms]' '{print ($1*60)+$2}')
    THROUGHPUT=$(echo "scale=2; $RECORDS / $SECONDS" | bc)

    echo "Tiempo: $TIME - Throughput: $THROUGHPUT reg/s"

    rm -f generated_data.csv
done
EOF

chmod +x benchmark.sh
./benchmark.sh
```

### Test 23: Análisis de Escalabilidad

```bash
# Test de escalabilidad con diferentes tamaños
cat > scalability.sh << 'EOF'
#!/bin/bash

echo "=== Análisis de Escalabilidad ==="
echo ""

GENS=4

for RECORDS in 100 1000 5000 10000 50000 100000; do
    echo -n "Registros: $RECORDS - "

    START=$(date +%s.%N)
    ./bin/DataGenerator $GENS $RECORDS > /dev/null 2>&1
    END=$(date +%s.%N)

    TIME=$(echo "$END - $START" | bc)
    THROUGHPUT=$(echo "scale=2; $RECORDS / $TIME" | bc)

    echo "Tiempo: ${TIME}s - Throughput: $THROUGHPUT reg/s"

    rm -f generated_data.csv
done
EOF

chmod +x scalability.sh
./scalability.sh
```

### Test 24: Stress Test

```bash
# Stress test: Ejecución repetida para detectar memory leaks
cat > stress.sh << 'EOF'
#!/bin/bash

echo "=== Stress Test ==="
echo "Ejecutando 100 iteraciones..."

for i in {1..100}; do
    echo -n "Iteración $i/100: "

    ./bin/DataGenerator 10 1000 > /dev/null 2>&1

    if [ $? -eq 0 ]; then
        echo "✅"
    else
        echo "❌ Fallo en iteración $i"
        exit 1
    fi

    # Verificar que no quedan recursos IPC
    SHM_COUNT=$(ipcs -m | grep $(whoami) | wc -l)
    SEM_COUNT=$(ipcs -s | grep $(whoami) | wc -l)

    if [ $SHM_COUNT -ne 0 ] || [ $SEM_COUNT -ne 0 ]; then
        echo "❌ Recursos IPC no liberados (SHM: $SHM_COUNT, SEM: $SEM_COUNT)"
        exit 1
    fi

    rm -f generated_data.csv
done

echo ""
echo "✅ Stress test completado: 100/100 iteraciones exitosas"
EOF

chmod +x stress.sh
./stress.sh
```

## Debugging

### Test 25: Compilar con Símbolos de Debug

```bash
# Modificar Makefile temporalmente
sed -i 's/CFLAGS = -Wall -Wextra -g/CFLAGS = -Wall -Wextra -g -O0 -DDEBUG/' Makefile

# Recompilar
make clean
make

# Ejecutar con gdb
gdb ./bin/DataGenerator

# En gdb:
(gdb) run 4 1000
(gdb) backtrace  # Si hay error
(gdb) info threads
(gdb) quit
```

### Test 26: Análisis con Valgrind

```bash
# Ejecutar con valgrind para detectar memory leaks
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
         ./bin/DataGenerator 2 100

# Analizar resultado
# Debe mostrar: "All heap blocks were freed -- no leaks are possible"
```

### Test 27: Tracing con strace

```bash
# Trace de system calls IPC
strace -f -e trace=ipc ./bin/DataGenerator 2 100 2>&1 | grep -E 'shmget|shmat|shmdt|shmctl|semget|semop|semctl'

# Debe mostrar:
# shmget(...) = X
# shmat(...) = ...
# semget(...) = Y
# semctl(...) = ...
# shmdt(...) = 0
# shmctl(...) = 0
# semctl(...) = 0
```

### Test 28: Análisis de Deadlocks

```bash
# Ejecutar con timeout para detectar posibles deadlocks
timeout 30s ./bin/DataGenerator 10 10000

if [ $? -eq 124 ]; then
    echo "❌ Timeout: Posible deadlock"
    # Obtener PIDs y analizar
    ps aux | grep DataGenerator
    # Verificar estado de semáforos
    ipcs -s
else
    echo "✅ No se detectó deadlock"
fi
```

## Script de Testing Completo

```bash
# Crear script maestro que ejecuta todos los tests
cat > run_all_tests.sh << 'EOF'
#!/bin/bash

echo "======================================"
echo "  Sistema de Testing Automatizado"
echo "======================================"
echo ""

PASSED=0
FAILED=0

run_test() {
    TEST_NAME=$1
    TEST_CMD=$2

    echo -n "Ejecutando $TEST_NAME... "

    if eval "$TEST_CMD" > /tmp/test_output.log 2>&1; then
        echo "✅ PASADO"
        ((PASSED++))
    else
        echo "❌ FALLIDO"
        ((FAILED++))
        cat /tmp/test_output.log
    fi
}

# Tests básicos
run_test "Test 1: Compilación" "make clean && make"
run_test "Test 2: Ejecución mínima" "./bin/DataGenerator 1 1"
run_test "Test 3: Ejecución estándar" "./bin/DataGenerator 4 1000"

# Tests de validación
run_test "Test 4: IDs únicos" "cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l | grep -q ^0$"
run_test "Test 5: Conteo correcto" "[ $(tail -n +2 generated_data.csv | wc -l) -eq 1000 ]"

# Tests de concurrencia
run_test "Test 6: Alta concurrencia" "./bin/DataGenerator 20 10000"

# Limpiar
rm -f generated_data.csv /tmp/test_output.log

echo ""
echo "======================================"
echo "  Resumen de Tests"
echo "======================================"
echo "✅ Tests Pasados: $PASSED"
echo "❌ Tests Fallidos: $FAILED"
echo "📊 Total: $((PASSED + FAILED))"
echo ""

if [ $FAILED -eq 0 ]; then
    echo "🎉 ¡Todos los tests pasaron!"
    exit 0
else
    echo "⚠️  Algunos tests fallaron"
    exit 1
fi
EOF

chmod +x run_all_tests.sh
./run_all_tests.sh
```

---

**Autor**: Agustin  
**Materia**: Sistemas Operativos - UNLAM  
**Fecha**: Octubre 2025
