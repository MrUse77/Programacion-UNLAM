# Sistema de Generación Distribuida de Datos CSV con IPC

Sistema de generación paralela de datos que utiliza múltiples procesos generadores que escriben concurrentemente en un archivo CSV coordinado, implementando comunicación entre procesos (IPC) mediante memoria compartida y semáforos POSIX.

## Características

- **Arquitectura Multi-Proceso**: Coordinador + N Generadores concurrentes
- **Comunicación IPC**: Memoria compartida y semáforos System V
- **Concurrencia**: Múltiples procesos generadores trabajando en paralelo
- **Sincronización**: Control de acceso mediante semáforos (mutex)
- **Buffer Compartido**: Comunicación eficiente entre generadores y coordinador
- **Generación de Datos**: Datos aleatorios realistas (nombres, ciudades, salarios)
- **Control de Señales**: Terminación controlada ante interrupciones
- **Gestión de Recursos**: Limpieza automática de IPC al finalizar

## Compilación

```bash
# Compilar el proyecto
make

# Compilar y limpiar todo
make clean
make

# Limpiar binarios y objetos
make clean
```

## Uso

### Ejecución Básica

```bash
# Sintaxis
./bin/DataGenerator <num_generadores> <total_registros>

# Ejemplo con 4 generadores y 1000 registros
./bin/DataGenerator 4 1000

# Ejemplo con 10 generadores y 5000 registros
./bin/DataGenerator 10 5000
```

### Usando el Makefile

```bash
# Ejecutar con configuración predeterminada (4 generadores, 321 registros)
make init

# Modificar valores en Makefile:
# REGISTERS = 1000
# CHILDS = 8
make init
```

### Parámetros

- **num_generadores**: Número de procesos generadores (1-20)
- **total_registros**: Total de registros a generar (1-100000)

## Arquitectura del Sistema

### Componentes Principales

1. **Proceso Principal**

   - Inicializa recursos IPC (memoria compartida y semáforos)
   - Crea procesos generadores
   - Ejecuta el coordinador
   - Espera a que terminen todos los procesos
   - Limpia recursos al finalizar

2. **Procesos Generadores** (N procesos)

   - Solicitan IDs de forma secuencial
   - Generan datos aleatorios para cada ID
   - Escriben en buffer de memoria compartida
   - Sincronizan acceso con semáforos

3. **Proceso Coordinador**
   - Lee datos del buffer compartido
   - Escribe en archivo CSV (`generated_data.csv`)
   - Controla duplicados
   - Procesa hasta completar todos los registros

### Flujo de Ejecución

```
┌──────────────┐
│  Proceso     │
│  Principal   │
└──────┬───────┘
       │
       ├─ Crear Memoria Compartida
       ├─ Crear Semáforos (IDs, Buffer, Write)
       │
       ├───────┬───────┬───────┬───────┐
       │       │       │       │       │
   ┌───▼──┐ ┌─▼───┐ ┌─▼───┐ ┌─▼───┐ ┌▼──────────┐
   │Gen 1 │ │Gen 2│ │Gen 3│ │Gen N│ │Coordinador│
   └───┬──┘ └─┬───┘ └─┬───┘ └─┬───┘ └┬──────────┘
       │      │       │       │      │
       └──────┴───────┴───────┴──────┘
                     │
              ┌──────▼─────────┐
              │ Buffer Memoria │
              │   Compartida   │
              └──────┬─────────┘
                     │
              ┌──────▼──────────┐
              │ generated_data  │
              │     .csv        │
              └─────────────────┘
```

## Memoria Compartida

### Estructura de Datos

```c
typedef struct {
    int nextId;           // Próximo ID a asignar
    int total;            // Total de registros a generar
    int regEscritos;      // Registros ya escritos en CSV
    int shutdown_flag;    // Flag de terminación
    int genActivos;       // Generadores activos
    char buffer[SHM_SIZE]; // Buffer de datos (25600 bytes)
    int bufferCount;      // Cantidad de registros en buffer
} sharedData;
```

## Sincronización con Semáforos

El sistema utiliza 3 semáforos para controlar el acceso a recursos compartidos:

### Semáforos Implementados

| Semáforo     | Propósito                    | Protege                 |
| ------------ | ---------------------------- | ----------------------- |
| `SEM_IDS`    | Control de asignación de IDs | `nextId`, `genActivos`  |
| `SEM_BUFFER` | Control de acceso al buffer  | `buffer`, `bufferCount` |
| `SEM_WRITE`  | Control de escritura en CSV  | Operaciones de archivo  |

### Flujo de Sincronización

1. **Generador solicita IDs**:

   ```
   wait(SEM_IDS)
   → Leer y actualizar nextId
   signal(SEM_IDS)
   ```

2. **Generador escribe en buffer**:

   ```
   wait(SEM_BUFFER)
   → Verificar espacio en buffer
   → Escribir datos en buffer
   signal(SEM_BUFFER)
   ```

3. **Coordinador procesa buffer**:

   ```
   wait(SEM_BUFFER)
   → Copiar buffer a local
   → Limpiar buffer compartido
   signal(SEM_BUFFER)
   → Procesar datos locales

   wait(SEM_WRITE)
   → Escribir en CSV
   signal(SEM_WRITE)
   ```

## Formato de Datos Generados

### Archivo CSV de Salida

El archivo `generated_data.csv` contiene registros con el siguiente formato:

```csv
ID,Name,Age,City,Department,Salary,Experience
1,Juan,45,Madrid,IT,87543,15
2,Maria,28,Barcelona,Sales,52341,8
3,Carlos,35,Valencia,Marketing,68921,12
...
```

### Campos de Datos

| Campo      | Tipo   | Rango          | Descripción                    |
| ---------- | ------ | -------------- | ------------------------------ |
| ID         | int    | 1 - total      | Identificador único secuencial |
| Name       | string | -              | Nombre aleatorio de 8 opciones |
| Age        | int    | 18 - 65        | Edad aleatoria                 |
| City       | string | -              | Ciudad de 6 opciones           |
| Department | string | -              | Departamento de 6 opciones     |
| Salary     | int    | 30000 - 100000 | Salario anual                  |
| Experience | int    | 0 - (Age-18)   | Años de experiencia            |

### Datos Aleatorios Disponibles

- **Nombres**: Juan, Maria, Carlos, Ana, Luis, Carmen, Pedro, Laura
- **Ciudades**: Madrid, Barcelona, Valencia, Sevilla, Bilbao, Málaga
- **Departamentos**: IT, HR, Sales, Marketing, Finance, Operations

## Control de Señales

El sistema maneja las siguientes señales para terminación controlada:

- **SIGINT** (Ctrl+C): Terminación por usuario
- **SIGTERM**: Terminación del sistema
- **SIGQUIT** (Ctrl+\\): Salida y core dump
- **SIGCHLD**: Manejo de procesos hijos terminados

### Terminación Controlada

Cuando se recibe una señal de interrupción:

1. Se establece `shutdown_flag = 1`
2. Los generadores terminan ordenadamente
3. El coordinador procesa datos pendientes
4. Se liberan recursos IPC:
   - Desacople de memoria compartida (`shmdt`)
   - Eliminación de memoria compartida (`shmctl IPC_RMID`)
   - Eliminación de semáforos (`semctl IPC_RMID`)
5. Se liberan estructuras de datos dinámicas

## Ejemplos de Uso

### Ejemplo 1: Generación Pequeña

```bash
$ ./bin/DataGenerator 2 100
Iniciando sistema con 2 generadores y 100 registros
PID del proceso principal: 12345
Generador 1 iniciado (PID: 12346)
Generador 2 iniciado (PID: 12347)
Total de registros procesados: 100
Generador 1 finalizado
Generador 2 finalizado

Sistema finalizado exitosamente
Total de registros generados: 100
Limpiando recursos del sistema...
```

### Ejemplo 2: Generación Grande con Múltiples Procesos

```bash
$ ./bin/DataGenerator 10 10000
Iniciando sistema con 10 generadores y 10000 registros
PID del proceso principal: 23456
Generador 1 iniciado (PID: 23457)
Generador 2 iniciado (PID: 23458)
...
Generador 10 iniciado (PID: 23466)
Total de registros procesados: 10000
...
Sistema finalizado exitosamente
Total de registros generados: 10000
```

### Ejemplo 3: Interrupción Controlada

```bash
$ ./bin/DataGenerator 5 5000
Iniciando sistema con 5 generadores y 5000 registros
...
^C
Señal 2 recibida. Iniciando terminación controlada...
Limpiando recursos del sistema...
```

## Monitoreo del Sistema

### Ver Memoria Compartida

```bash
# Listar segmentos de memoria compartida
ipcs -m

# Ver detalles de un segmento específico
ipcs -m -i <shmid>
```

### Ver Semáforos

```bash
# Listar semáforos
ipcs -s

# Ver detalles de un conjunto de semáforos
ipcs -s -i <semid>
```

### Limpiar Recursos IPC Manualmente (si es necesario)

```bash
# Eliminar memoria compartida
ipcrm -m <shmid>

# Eliminar semáforos
ipcrm -s <semid>

# Eliminar todos los recursos IPC del usuario
ipcrm -a
```

### Monitoreo de Procesos

```bash
# Ver árbol de procesos
pstree -p <pid_principal>

# Ver procesos en tiempo real
watch -n 1 'ps aux | grep DataGenerator'

# Ver estadísticas de procesos
ps -eLf | grep DataGenerator
```

## Estructura del Proyecto

```
Ejercicio 1 TP/
├── src/
│   ├── main.c              # Proceso principal, inicialización IPC
│   ├── coordinator.c       # Proceso coordinador
│   ├── generators.c        # Procesos generadores
│   └── includes/
│       ├── variables.h     # Definiciones y estructuras
│       ├── coordinator.h   # Interface del coordinador
│       └── generator.h     # Interface de generadores
├── obj/                    # Archivos objeto (.o)
├── bin/
│   └── DataGenerator       # Binario ejecutable
├── Makefile               # Sistema de compilación
├── generated_data.csv     # Archivo de salida
└── README.md              # Esta documentación
```

## Requisitos

- **Sistema Operativo**: Linux/Unix
- **Compilador**: GCC (GNU Compiler Collection)
- **Bibliotecas**:
  - POSIX threads
  - System V IPC (memoria compartida, semáforos)
  - Standard C library

## Solución de Problemas

### Error: "No such file or directory"

```bash
# Asegurarse de que exista el directorio bin/
mkdir -p bin obj
make
```

### Error: "Permission denied"

```bash
# Dar permisos de ejecución
chmod +x bin/DataGenerator
```

### Recursos IPC no liberados

```bash
# Verificar recursos IPC
ipcs

# Limpiar todo
ipcrm -a

# O específicamente
ipcrm -m <shmid>
ipcrm -s <semid>
```

### Procesos zombies

```bash
# Ver procesos zombies
ps aux | grep defunct

# Matar procesos huérfanos
pkill -9 DataGenerator
```

## Optimización y Rendimiento

### Parámetros Configurables

En `variables.h` se pueden ajustar:

```c
#define MAX_GENERATORS 20        // Máximo de generadores
#define MAX_RECORDS 100000       // Máximo de registros
#define RECORD_SIZE 256          // Tamaño de cada registro
#define SHM_SIZE (RECORD_SIZE * 100) // Tamaño del buffer
#define IDS_PER_REQUEST 10       // IDs por solicitud
```

### Recomendaciones

- **Buffer grande**: Para muchos generadores, aumentar `SHM_SIZE`
- **Más IDs por request**: Reducir contención con `IDS_PER_REQUEST` mayor
- **Balance generadores**: El óptimo suele ser igual al número de CPUs

## Conceptos de Sistemas Operativos Implementados

### IPC (Inter-Process Communication)

- ✅ Memoria compartida (shared memory)
- ✅ Semáforos System V
- ✅ Comunicación productor-consumidor

### Concurrencia

- ✅ Múltiples procesos concurrentes
- ✅ Sincronización con semáforos
- ✅ Secciones críticas
- ✅ Mutex (exclusión mutua)

### Gestión de Procesos

- ✅ `fork()` - Creación de procesos
- ✅ `wait()`/`waitpid()` - Espera de procesos hijo
- ✅ Manejo de señales
- ✅ Procesos zombie y huérfanos

### Gestión de Recursos

- ✅ Asignación dinámica de memoria
- ✅ Limpieza de recursos IPC
- ✅ Manejadores de señales para cleanup

## Materia

Sistemas Operativos - UNLAM

## Fecha

Octubre 2025

---

**Documentación Completa**:

- **README.md** (este archivo): Manual completo de usuario
- **ARCHITECTURE.md**: Documentación técnica detallada
- **QUICKSTART.md**: Guía rápida de inicio
- **RESUMEN.md**: Resumen ejecutivo del proyecto
