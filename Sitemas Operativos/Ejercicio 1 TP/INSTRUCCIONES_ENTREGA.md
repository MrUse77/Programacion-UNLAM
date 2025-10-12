# Instrucciones de Entrega y Evaluación

## Para el Evaluador

Este documento contiene toda la información necesaria para compilar, ejecutar y evaluar el **Ejercicio 1: Sistema de Generación Distribuida de Datos CSV con IPC**.

---

## Inicio Rápido (60 segundos)

```bash
# 1. Navegar al directorio
cd "Ejercicio 1 TP"

# 2. Compilar
make

# 3. Ejecutar prueba rápida
./bin/DataGenerator 4 1000

# 4. Verificar resultado
head generated_data.csv
wc -l generated_data.csv  # Debe mostrar 1001 líneas
```

**Resultado esperado**: El programa crea un archivo `generated_data.csv` con exactamente 1000 registros de datos + 1 línea de header.

---

## Requisitos del Sistema

### Software Necesario

- **Sistema Operativo**: Linux (kernel 2.6+)
- **Compilador**: GCC 4.8 o superior
- **Make**: GNU Make
- **Bibliotecas**:
  - libc (Standard C Library)
  - System V IPC support (memoria compartida y semáforos)

### Verificar Requisitos

```bash
# Verificar GCC
gcc --version

# Verificar make
make --version

# Verificar soporte IPC
ipcs -l
```

---

## Compilación

### Compilación Estándar

```bash
make
```

### Compilación desde Cero

```bash
make clean
make
```

### Compilación con Debug

```bash
# Editar Makefile y agregar -DDEBUG
make clean
CFLAGS="-Wall -Wextra -g -DDEBUG" make
```

**Archivos Generados**:

- `bin/DataGenerator` - Ejecutable principal
- `obj/*.o` - Archivos objeto

---

## Ejecución

### Sintaxis

```bash
./bin/DataGenerator <num_generadores> <total_registros>
```

### Parámetros

| Parámetro       | Rango    | Descripción                                 |
| --------------- | -------- | ------------------------------------------- |
| num_generadores | 1-20     | Número de procesos generadores concurrentes |
| total_registros | 1-100000 | Total de registros a generar                |

### Ejemplos de Uso

```bash
# Ejemplo 1: Configuración pequeña
./bin/DataGenerator 2 100

# Ejemplo 2: Configuración media (recomendada para evaluación)
./bin/DataGenerator 4 1000

# Ejemplo 3: Configuración grande
./bin/DataGenerator 10 10000

# Ejemplo 4: Máximo permitido
./bin/DataGenerator 20 100000
```

### Usando el Makefile

```bash
# Compilar y ejecutar con configuración predeterminada
make init

# Los valores por defecto están en el Makefile:
# REGISTERS = 321
# CHILDS = 4
```

---

## Verificación de Funcionamiento

### 1. Verificar Archivo Generado

```bash
# Ver primeros registros
head generated_data.csv

# Contar registros (debe ser N+1, incluyendo header)
wc -l generated_data.csv

# Verificar formato
cat generated_data.csv | head -5
```

**Ejemplo de salida esperada**:

```csv
ID,Name,Age,City,Department,Salary,Experience
1,Juan,45,Madrid,IT,87543,15
2,Maria,28,Barcelona,Sales,52341,8
3,Carlos,35,Valencia,Marketing,68921,12
4,Ana,52,Sevilla,HR,73456,25
```

### 2. Verificar IDs Únicos

```bash
# Extraer IDs y buscar duplicados
cut -d',' -f1 generated_data.csv | tail -n +2 | sort -n | uniq -d

# No debe mostrar nada (sin duplicados)
```

### 3. Verificar Conteo de Registros

```bash
# Contar registros (sin header)
EXPECTED=1000  # Cambiar según lo que ejecutaste
ACTUAL=$(tail -n +2 generated_data.csv | wc -l)

if [ $ACTUAL -eq $EXPECTED ]; then
    echo "✅ Conteo correcto: $ACTUAL registros"
else
    echo "❌ Error: Se esperaban $EXPECTED, se obtuvieron $ACTUAL"
fi
```

### 4. Verificar Limpieza de Recursos IPC

```bash
# Después de ejecutar el programa, verificar que no quedan recursos
ipcs -m  # Memoria compartida
ipcs -s  # Semáforos

# No debe haber segmentos del usuario sin dueño
```

---

## Casos de Prueba Sugeridos

### Caso 1: Validación de Parámetros

```bash
# Parámetros inválidos (deben fallar con mensaje de error)
./bin/DataGenerator
./bin/DataGenerator 0 1000
./bin/DataGenerator 25 1000
./bin/DataGenerator 4 0
./bin/DataGenerator 4 150000
```

**Resultado esperado**: Cada comando debe mostrar un mensaje de error descriptivo.

### Caso 2: Ejecución Mínima

```bash
./bin/DataGenerator 1 1
```

**Verificar**:

- ✅ 1 proceso generador creado
- ✅ Archivo con 2 líneas (header + 1 dato)
- ✅ ID = 1

### Caso 3: Ejecución Estándar

```bash
./bin/DataGenerator 4 1000
```

**Verificar**:

- ✅ 4 procesos generadores
- ✅ 1001 líneas en archivo
- ✅ IDs del 1 al 1000, sin duplicados
- ✅ Todos los registros tienen 7 campos

### Caso 4: Alta Concurrencia

```bash
./bin/DataGenerator 20 50000
```

**Verificar**:

- ✅ 20 procesos generadores
- ✅ 50001 líneas en archivo
- ✅ Sin IDs duplicados
- ✅ Tiempo de ejecución razonable

### Caso 5: Interrupción Controlada

```bash
# Ejecutar
./bin/DataGenerator 10 100000

# Inmediatamente presionar Ctrl+C
```

**Verificar**:

- ✅ Mensaje de terminación controlada
- ✅ Recursos IPC liberados (ipcs -m -s)
- ✅ No quedan procesos zombie

---

## Scripts de Verificación Automatizada

### Script 1: Verificación Básica

```bash
#!/bin/bash

echo "=== Verificación Básica ==="

# Compilar
echo "Compilando..."
make clean && make
if [ $? -ne 0 ]; then
    echo "❌ Error de compilación"
    exit 1
fi
echo "✅ Compilación exitosa"

# Ejecutar
echo "Ejecutando..."
./bin/DataGenerator 4 1000
if [ $? -ne 0 ]; then
    echo "❌ Error de ejecución"
    exit 1
fi
echo "✅ Ejecución exitosa"

# Verificar conteo
LINES=$(wc -l < generated_data.csv)
if [ $LINES -eq 1001 ]; then
    echo "✅ Conteo correcto: 1001 líneas"
else
    echo "❌ Conteo incorrecto: $LINES líneas (esperadas: 1001)"
    exit 1
fi

# Verificar duplicados
DUPES=$(cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l)
if [ $DUPES -eq 0 ]; then
    echo "✅ Sin IDs duplicados"
else
    echo "❌ $DUPES IDs duplicados"
    exit 1
fi

echo ""
echo "🎉 ¡Todas las verificaciones pasaron!"
```

### Script 2: Test Completo

```bash
#!/bin/bash

echo "=== Test Completo del Sistema ==="

TESTS_PASSED=0
TESTS_FAILED=0

test_case() {
    NAME=$1
    GENS=$2
    RECS=$3

    echo -n "Test: $NAME ($GENS gen, $RECS rec)... "

    ./bin/DataGenerator $GENS $RECS > /dev/null 2>&1

    if [ $? -ne 0 ]; then
        echo "❌ Fallo en ejecución"
        ((TESTS_FAILED++))
        return
    fi

    LINES=$(tail -n +2 generated_data.csv | wc -l)
    DUPES=$(cut -d',' -f1 generated_data.csv | tail -n +2 | sort | uniq -d | wc -l)

    if [ $LINES -eq $RECS ] && [ $DUPES -eq 0 ]; then
        echo "✅ OK"
        ((TESTS_PASSED++))
    else
        echo "❌ FALLO (líneas: $LINES/$RECS, dupes: $DUPES)"
        ((TESTS_FAILED++))
    fi

    rm -f generated_data.csv
}

# Compilar
make clean && make > /dev/null 2>&1

# Ejecutar tests
test_case "Mínimo" 1 1
test_case "Pequeño" 2 100
test_case "Medio" 4 1000
test_case "Grande" 10 10000
test_case "Máxima concurrencia" 20 5000

echo ""
echo "=== Resultados ==="
echo "✅ Tests pasados: $TESTS_PASSED"
echo "❌ Tests fallidos: $TESTS_FAILED"
echo "📊 Total: $((TESTS_PASSED + TESTS_FAILED))"

if [ $TESTS_FAILED -eq 0 ]; then
    echo "🎉 ¡Todos los tests pasaron!"
    exit 0
else
    echo "⚠️ Algunos tests fallaron"
    exit 1
fi
```

---

## Criterios de Evaluación

### Funcionalidad (40%)

- ✅ **Generación de datos**: Crea archivo CSV con N registros
- ✅ **IDs únicos**: Sin duplicados, rango [1, N]
- ✅ **Formato correcto**: Header + N líneas, 7 campos por línea
- ✅ **Datos aleatorios**: Valores en rangos especificados

### IPC y Sincronización (40%)

- ✅ **Memoria compartida**: Uso correcto de shmget/shmat/shmdt
- ✅ **Semáforos**: 3 semáforos para sincronización
- ✅ **Secciones críticas**: Protección correcta de recursos
- ✅ **Sin race conditions**: IDs únicos garantizados
- ✅ **Sin deadlocks**: Sistema siempre termina

### Gestión de Procesos (10%)

- ✅ **fork()**: Creación correcta de N generadores
- ✅ **wait()**: Espera correcta de procesos hijo
- ✅ **Señales**: Manejo de SIGINT, SIGTERM
- ✅ **Cleanup**: Liberación de recursos IPC

### Robustez (10%)

- ✅ **Validación**: Parámetros validados correctamente
- ✅ **Manejo de errores**: System calls verificados
- ✅ **Terminación controlada**: Cleanup ante interrupciones
- ✅ **Sin leaks**: Memoria y recursos liberados

---

## Documentación Incluida

El proyecto incluye documentación completa:

1. **README.md** (~15 KB)

   - Manual completo de usuario
   - Ejemplos de uso
   - Descripción de arquitectura
   - Solución de problemas

2. **ARCHITECTURE.md** (~25 KB)

   - Documentación técnica detallada
   - Diagramas de flujo
   - Análisis de secciones críticas
   - Garantías del sistema

3. **QUICKSTART.md** (~5 KB)

   - Guía rápida de inicio (60 segundos)
   - Ejemplos de comandos
   - Verificación rápida

4. **RESUMEN.md** (~10 KB)

   - Resumen ejecutivo del proyecto
   - Características implementadas
   - Conceptos de SO aplicados
   - Métricas de rendimiento

5. **TESTING.md** (~12 KB)

   - Guía completa de testing
   - Scripts de verificación
   - Tests de concurrencia y señales
   - Debugging y profiling

6. **INSTRUCCIONES_ENTREGA.md** (este archivo)
   - Guía para el evaluador
   - Casos de prueba
   - Scripts de verificación

---

## Limpieza

### Limpiar Binarios

```bash
make clean
```

### Limpiar Todo (incluyendo datos generados)

```bash
make clean
rm -f generated_data.csv
```

### Limpiar Recursos IPC (si es necesario)

```bash
# Ver recursos IPC
ipcs -m -s

# Limpiar todo del usuario
ipcrm -a

# O específicamente
ipcrm -m <shmid>
ipcrm -s <semid>
```

---

## Estructura del Proyecto

```
Ejercicio 1 TP/
├── src/
│   ├── main.c                    # Proceso principal
│   ├── coordinator.c             # Proceso coordinador
│   ├── generators.c              # Procesos generadores
│   └── includes/
│       ├── variables.h           # Estructuras y constantes
│       ├── coordinator.h         # Interface coordinador
│       └── generator.h           # Interface generadores
├── obj/                          # Archivos objeto (.o)
├── bin/
│   └── DataGenerator             # Ejecutable
├── Makefile                      # Sistema de compilación
├── generated_data.csv            # Archivo de salida (generado)
├── README.md                     # Manual completo
├── ARCHITECTURE.md               # Documentación técnica
├── QUICKSTART.md                 # Guía rápida
├── RESUMEN.md                    # Resumen ejecutivo
├── TESTING.md                    # Guía de testing
└── INSTRUCCIONES_ENTREGA.md      # Este archivo
```

---

## Contacto y Soporte

**Autor**: Agustin  
**Materia**: Sistemas Operativos  
**Universidad**: UNLAM  
**Fecha**: Octubre 2025

Para consultas sobre el proyecto, referirse a la documentación incluida o contactar al autor.

---

## Checklist de Evaluación

### Antes de Evaluar

- [ ] Sistema Linux disponible
- [ ] GCC y make instalados
- [ ] Soporte para System V IPC verificado
- [ ] Permisos para crear recursos IPC

### Durante la Evaluación

- [ ] Compilación exitosa
- [ ] Ejecución sin errores
- [ ] Archivo CSV generado correctamente
- [ ] IDs únicos verificados
- [ ] Conteo de registros correcto
- [ ] Recursos IPC liberados
- [ ] Sin procesos zombie
- [ ] Documentación revisada

### Después de Evaluar

- [ ] Limpiar binarios (make clean)
- [ ] Eliminar archivos generados
- [ ] Verificar limpieza de IPC (ipcs)

---

**Última actualización**: Octubre 2025

✅ **Proyecto completo y listo para evaluación**
