# 🚀 Inicio Rápido - 60 Segundos

## Compilar y Ejecutar

```bash
# 1. Compilar (5 segundos)
cd "Ejercicio 1 TP"
make

# 2. Ejecutar con 4 generadores y 1000 registros
./bin/DataGenerator 4 1000

# 3. Ver resultado
head generated_data.csv
wc -l generated_data.csv   # Debe mostrar 1001 líneas (1 header + 1000 datos)

# 4. Limpiar
make clean
```

## Ejemplo de Salida

```bash
$ ./bin/DataGenerator 4 1000
Iniciando sistema con 4 generadores y 1000 registros
PID del proceso principal: 12345
Generador 1 iniciado (PID: 12346)
Generador 2 iniciado (PID: 12347)
Generador 3 iniciado (PID: 12348)
Generador 4 iniciado (PID: 12349)
Total de registros procesados: 1000
Generador 1 finalizado
Generador 2 finalizado
Generador 3 finalizado
Generador 4 finalizado

Sistema finalizado exitosamente
Total de registros generados: 1000
Limpiando recursos del sistema...
```

## Verificar Datos Generados

```bash
# Ver primeros 10 registros
$ head generated_data.csv
ID,Name,Age,City,Department,Salary,Experience
1,Juan,45,Madrid,IT,87543,15
2,Maria,28,Barcelona,Sales,52341,8
3,Carlos,35,Valencia,Marketing,68921,12
4,Ana,52,Sevilla,HR,73456,25
5,Luis,29,Bilbao,Finance,48723,7
...

# Contar registros totales (sin contar header)
$ tail -n +2 generated_data.csv | wc -l
1000

# Verificar que todos los IDs son únicos
$ cut -d',' -f1 generated_data.csv | tail -n +2 | sort -n | uniq -d
(no debe mostrar nada si todos son únicos)
```

## Prueba con Diferentes Configuraciones

```bash
# Pocos generadores, pocos datos
./bin/DataGenerator 2 100

# Muchos generadores, muchos datos
./bin/DataGenerator 10 10000

# Máximo permitido
./bin/DataGenerator 20 100000
```

## Monitorear Ejecución en Tiempo Real

```bash
# Terminal 1: Ejecutar programa
./bin/DataGenerator 8 50000

# Terminal 2: Ver recursos IPC
watch -n 1 'ipcs -m -s'

# Terminal 3: Ver archivo creciendo
watch -n 1 'wc -l generated_data.csv'

# Terminal 4: Ver procesos
watch -n 1 'pstree -p $(pgrep -f DataGenerator | head -1)'
```

## Prueba de Interrupción Controlada

```bash
# 1. Iniciar generación grande
./bin/DataGenerator 5 50000

# 2. Interrumpir con Ctrl+C
^C
Señal 2 recibida. Iniciando terminación controlada...
Limpiando recursos del sistema...

# 3. Verificar que no quedan recursos IPC huérfanos
ipcs -m -s
# No debe haber segmentos sin owner
```

## Makefile Quick Commands

```bash
# Compilar
make

# Compilar y ejecutar con defaults (4 generadores, 321 registros)
make init

# Limpiar todo
make clean

# Limpiar y recompilar
make clean && make
```

## Modificar Configuración en Makefile

Editar `Makefile`:

```makefile
# Cambiar estos valores
REGISTERS = 5000    # Número de registros
CHILDS = 8          # Número de generadores

# Luego ejecutar
make init
```

## Verificar Instalación

```bash
# Verificar compilador
gcc --version

# Verificar soporte IPC
ipcs -l

# Compilar en modo verbose
make clean
make CC=gcc CFLAGS="-Wall -Wextra -g -v"
```

## Solución Rápida de Problemas

### Error: "No such file or directory"

```bash
mkdir -p bin obj
make
```

### Error: "Permission denied"

```bash
chmod +x bin/DataGenerator
```

### Recursos IPC no limpiados

```bash
# Ver recursos
ipcs -a

# Limpiar todo del usuario
ipcrm -a

# O específicamente
ipcrm -m <shmid>
ipcrm -s <semid>
```

## Benchmarking Rápido

```bash
# Medir tiempo de ejecución
time ./bin/DataGenerator 4 10000

# Comparar diferentes configuraciones
echo "=== 2 generadores ==="
time ./bin/DataGenerator 2 10000

echo "=== 4 generadores ==="
time ./bin/DataGenerator 4 10000

echo "=== 8 generadores ==="
time ./bin/DataGenerator 8 10000
```

## Análisis de Datos Generados

```bash
# Estadísticas del archivo
$ wc -l -c generated_data.csv
  1001 líneas, 98543 bytes

# Distribución por departamento
$ cut -d',' -f5 generated_data.csv | tail -n +2 | sort | uniq -c
    167 Finance
    165 HR
    168 IT
    165 Marketing
    170 Operations
    165 Sales

# Salario promedio
$ awk -F',' 'NR>1 {sum+=$6; count++} END {print sum/count}' generated_data.csv
65234.7

# Edad promedio
$ awk -F',' 'NR>1 {sum+=$3; count++} END {print sum/count}' generated_data.csv
41.3
```

## Ejecutar Tests Específicos

```bash
# Test 1: Validación de parámetros
./bin/DataGenerator          # Debe mostrar ayuda
./bin/DataGenerator 0 1000   # Error: generadores inválidos
./bin/DataGenerator 25 1000  # Error: máximo 20 generadores
./bin/DataGenerator 4 0      # Error: registros inválidos
./bin/DataGenerator 4 150000 # Error: máximo 100000 registros

# Test 2: Ejecución normal
./bin/DataGenerator 4 1000   # Debe funcionar

# Test 3: Extremos
./bin/DataGenerator 1 1      # Mínimo
./bin/DataGenerator 20 100000 # Máximo
```

## Ayuda Rápida

```bash
# Ver ayuda del programa
./bin/DataGenerator

# Ver opciones del Makefile
make help  # (si está definido)

# Ver documentación completa
cat README.md
cat ARCHITECTURE.md
```

## Limpieza Total

```bash
# Limpiar binarios
make clean

# Limpiar archivo generado
rm -f generated_data.csv

# Limpiar recursos IPC
ipcrm -a

# Todo junto
make clean && rm -f generated_data.csv && ipcrm -a
```

## Documentación Completa

- **README.md**: Manual completo de usuario
- **ARCHITECTURE.md**: Documentación técnica detallada
- **QUICKSTART.md**: Esta guía rápida
- **RESUMEN.md**: Resumen ejecutivo del proyecto

## Requisitos Mínimos

- Linux con kernel 2.6+
- GCC 4.8+
- make
- Soporte para System V IPC (memoria compartida y semáforos)

---

✅ **¡Todo listo!** Ver README.md para documentación completa y detalles técnicos.

🔧 **¿Problemas?** Consultar la sección de solución de problemas en README.md

📚 **¿Quieres saber más?** Lee ARCHITECTURE.md para detalles de implementación
