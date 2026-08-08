# Documentación Técnica - Arquitectura y Protocolo

## Arquitectura del Sistema

### Modelo Multi-Proceso con IPC

```
┌─────────────────────────────────────────────────────────────────┐
│                      PROCESO PRINCIPAL                           │
│                                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │  main()                                                   │  │
│  │  - Validar parámetros                                     │  │
│  │  - Crear memoria compartida (shmget/shmat)                │  │
│  │  - Crear semáforos (semget)                               │  │
│  │  - Configurar manejadores de señales                      │  │
│  │  - fork() N generadores                                   │  │
│  │  - Ejecutar coordinador                                   │  │
│  │  - wait() procesos hijos                                  │  │
│  │  - Limpiar recursos IPC                                   │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
└──────────────────────┬───────────────────────────────────────────┘
                       │
         ┌─────────────┼─────────────┬──────────────┐
         │             │             │              │
    ┌────▼────┐   ┌───▼────┐   ┌───▼────┐   ┌────▼────────┐
    │  Gen 1  │   │  Gen 2 │...│  Gen N │   │ Coordinador │
    │ (fork)  │   │ (fork) │   │ (fork) │   │  (proceso)  │
    └────┬────┘   └───┬────┘   └───┬────┘   └────┬────────┘
         │            │            │              │
         │  ESCRITURA │            │    LECTURA   │
         └────────────┴────────────┴──────────────┘
                      │
         ┌────────────▼───────────────┐
         │   MEMORIA COMPARTIDA       │
         │  (System V Shared Memory)  │
         │                            │
         │  struct sharedData {       │
         │    int nextId              │
         │    int total               │
         │    int regEscritos         │
         │    char buffer[25600]      │
         │    ...                     │
         │  }                         │
         └────────────┬───────────────┘
                      │
         ┌────────────▼───────────────┐
         │   SEMÁFOROS (System V)     │
         │  - SEM_IDS    (mutex)      │
         │  - SEM_BUFFER (mutex)      │
         │  - SEM_WRITE  (mutex)      │
         └────────────┬───────────────┘
                      │
         ┌────────────▼───────────────┐
         │  generated_data.csv        │
         │  (Archivo de salida)       │
         └────────────────────────────┘
```

### Flujo de Datos Detallado

```
GENERADOR                    MEMORIA COMPARTIDA              COORDINADOR
   │                                 │                            │
   ├──[1. Solicitar IDs]────────────>│                            │
   │   wait(SEM_IDS)                 │                            │
   │   ids = nextId; nextId += N     │                            │
   │   signal(SEM_IDS)               │                            │
   │                                 │                            │
   ├──[2. Generar Datos]             │                            │
   │   generateRecord(id, data)      │                            │
   │                                 │                            │
   ├──[3. Escribir Buffer]──────────>│                            │
   │   wait(SEM_BUFFER)              │                            │
   │   strcat(buffer, data)          │                            │
   │   signal(SEM_BUFFER)            │                            │
   │                                 │                            │
   │                                 │<────[4. Leer Buffer]───────┤
   │                                 │    wait(SEM_BUFFER)        │
   │                                 │    localBuf = buffer       │
   │                                 │    buffer = ""             │
   │                                 │    signal(SEM_BUFFER)      │
   │                                 │                            │
   │                                 │    [5. Procesar Datos]     │
   │                                 │    parse(localBuf)         │
   │                                 │                            │
   │                                 │    [6. Escribir CSV]───────>
   │                                 │    wait(SEM_WRITE)         │
   │                                 │    fprintf(csv, data)      │
   │                                 │    regEscritos++           │
   │                                 │    signal(SEM_WRITE)       │
   │                                 │                            │
   └──[7. Repetir hasta total]───────┘                            │
                                     │                            │
                              [8. Verificar completitud]          │
                              while (regEscritos < total)         │
```

## Componentes del Sistema

### 1. Proceso Principal (main.c)

#### Responsabilidades

- Inicialización del sistema
- Creación de recursos IPC
- Gestión del ciclo de vida de procesos
- Limpieza de recursos

#### Funciones Principales

##### `main()`

```c
int main(int argc, char *argv[])
{
    // 1. Validación de parámetros
    validate_parameters(argc, argv, &cantGen, &total);

    // 2. Configurar señales
    asignarSignals();

    // 3. Crear y configurar IPC
    crearMemoriaCompartida();
    initMemoriaCompartida();
    crearSemaforos();
    initSemaforos();

    // 4. Crear procesos generadores
    for (int i = 0; i < cantGen; i++) {
        fork() → generator_process()
    }

    // 5. Ejecutar coordinador
    coordinator_process();

    // 6. Esperar terminación
    wait() todos los hijos

    // 7. Limpiar recursos
    cleanup_resources();
}
```

##### `crearMemoriaCompartida()`

```c
void crearMemoriaCompartida()
{
    // Crear segmento de memoria compartida
    shm_id = shmget(IPC_PRIVATE, sizeof(sharedData),
                    IPC_CREAT | 0666);

    // Adjuntar segmento al espacio de direcciones
    data = (sharedData *)shmat(shm_id, NULL, 0);

    // Inicializar estructura
    data->nextId = 1;
    data->total = total;
    data->genActivos = cantGen;
    data->shutdown_flag = 0;
}
```

##### `crearSemaforos()`

```c
void crearSemaforos()
{
    // Crear conjunto de 3 semáforos
    sem_id = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | 0666);

    // Inicializar cada semáforo a 1 (mutex libre)
    for (int i = 0; i < SEM_COUNT; i++) {
        semctl(sem_id, i, SETVAL, 1);
    }
}
```

##### `cleanup_resources()`

```c
void cleanup_resources()
{
    // 1. Marcar shutdown
    data->shutdown_flag = 1;

    // 2. Desacoplar memoria compartida
    shmdt(data);

    // 3. Eliminar segmento de memoria
    shmctl(shm_id, IPC_RMID, NULL);

    // 4. Eliminar semáforos
    semctl(sem_id, 0, IPC_RMID);

    // 5. Liberar memoria dinámica
    free(generador);
}
```

### 2. Procesos Generadores (generators.c)

#### Responsabilidades

- Solicitar IDs secuenciales
- Generar datos aleatorios
- Escribir en buffer compartido
- Sincronizar acceso con semáforos

#### Función Principal

##### `generator_process()`

```c
void generator_process(int genId, sharedData *data)
{
    while (!data->shutdown_flag) {
        // 1. Obtener lote de IDs
        int count = getNextIds(ids, IDS_PER_REQUEST, data);
        if (count == 0) break;

        // 2. Generar registros
        for (int i = 0; i < count; i++) {
            generateRecord(ids[i], reg);

            // 3. Enviar a buffer (con reintentos)
            while (!sendToBuffer(reg, data)) {
                // Esperar si buffer lleno
            }
        }
    }

    // 4. Decrementar contador de generadores activos
    wait(SEM_IDS);
    data->genActivos--;
    signal(SEM_IDS);
}
```

##### `getNextIds()` - Asignación de IDs

```c
int getNextIds(int *ids, int maxCount, sharedData *data)
{
    // SECCIÓN CRÍTICA: Asignación de IDs
    wait(SEM_IDS);

    if (data->nextId > data->total) {
        signal(SEM_IDS);
        return 0;  // No más IDs
    }

    // Asignar lote de IDs
    int assigned = 0;
    for (int i = 0; i < maxCount && data->nextId <= data->total; i++) {
        ids[i] = data->nextId++;
        assigned++;
    }

    signal(SEM_IDS);
    return assigned;
}
```

**Invariantes**:

- `nextId` siempre crece monotónicamente
- Nunca se asigna el mismo ID dos veces
- Los IDs son secuenciales desde 1 hasta `total`

##### `generateRecord()` - Generación de Datos

```c
void generateRecord(int id, char *record)
{
    // Semilla aleatoria única por proceso e ID
    srand(time(NULL) + getpid() + id);

    // Generar datos aleatorios
    int age = 18 + rand() % 48;              // 18-65 años
    int salario = 30000 + rand() % 70000;    // 30k-100k
    int experiencia = rand() % (age - 18 + 1); // 0 a (edad-18)

    // Formato CSV
    snprintf(record, RECORD_SIZE,
             "%d,%s,%d,%s,%s,%d,%d\n",
             id,
             names[rand() % 8],
             age,
             cities[rand() % 6],
             departments[rand() % 6],
             salario,
             experiencia);
}
```

**Características de Aleatoriedad**:

- Semilla única: `time(NULL) + getpid() + id`
- Evita repetición entre procesos y entre registros
- Rangos realistas de datos

##### `sendToBuffer()` - Escritura en Buffer

```c
int sendToBuffer(char *record, sharedData *data)
{
    // SECCIÓN CRÍTICA: Escritura en buffer
    wait(SEM_BUFFER);

    int len = strlen(record);

    // Verificar espacio disponible
    if (strlen(data->buffer) + len < SHM_SIZE - 1) {
        strcat(data->buffer, record);
        data->bufferCount++;
        signal(SEM_BUFFER);
        return 1;  // Éxito
    }

    signal(SEM_BUFFER);
    return 0;  // Buffer lleno, reintentar
}
```

**Control de Flujo**:

- Si buffer lleno → retorna 0 → generador reintenta
- Si hay espacio → escribe y retorna 1
- Evita pérdida de datos
- Productor se bloquea si consumidor es lento

### 3. Proceso Coordinador (coordinator.c)

#### Responsabilidades

- Leer datos del buffer compartido
- Procesar y validar registros
- Escribir en archivo CSV
- Controlar completitud de datos

#### Función Principal

##### `coordinator_process()`

```c
void coordinator_process(sharedData *data)
{
    // 1. Crear archivo CSV
    FILE *csv = fopen("generated_data.csv", "w");
    fprintf(csv, "ID,Name,Age,City,Department,Salary,Experience\n");

    // 2. Array para control de duplicados
    int *regProcesados = calloc(data->total, sizeof(int));

    // 3. Buffer local para procesar datos
    char localBuffer[SHM_SIZE];

    // 4. Loop principal de procesamiento
    while (data->regEscritos < data->total &&
           (data->genActivos > 0 || strlen(data->buffer) > 0)) {

        // SECCIÓN CRÍTICA: Leer buffer
        wait(SEM_BUFFER);
        if (strlen(data->buffer) > 0) {
            strcpy(localBuffer, data->buffer);
            memset(data->buffer, 0, SHM_SIZE);
            signal(SEM_BUFFER);

            // Procesar fuera de sección crítica
            processRecords(localBuffer, csv, regProcesados, data);
        } else {
            signal(SEM_BUFFER);
        }
    }

    fclose(csv);
}
```

**Optimización Clave**: El coordinador copia el buffer a memoria local y libera el mutex inmediatamente, permitiendo que los generadores escriban mientras se procesa.

##### Procesamiento de Registros

```c
void processRecords(char *localBuffer, FILE *csv,
                   int *regProcesados, sharedData *data)
{
    char *line = strtok(localBuffer, "\n");

    while (line != NULL) {
        int id;
        if (sscanf(line, "%d,", &id) == 1) {
            // Verificar duplicados
            if (id >= 1 && id <= data->total && !regProcesados[id]) {
                // SECCIÓN CRÍTICA: Escritura en archivo
                wait(SEM_WRITE);
                fprintf(csv, "%s\n", line);
                fflush(csv);
                data->regEscritos++;
                regProcesados[id] = 1;
                signal(SEM_WRITE);
            }
        }
        line = strtok(NULL, "\n");
    }
}
```

**Control de Duplicados**:

- Array `regProcesados[]` marca IDs ya escritos
- Evita escribir el mismo ID múltiples veces
- Garantiza exactamente `total` registros únicos

## Sincronización y Concurrencia

### Análisis de Secciones Críticas

#### Sección Crítica 1: Asignación de IDs

```
Recurso Compartido: data->nextId
Semáforo: SEM_IDS
Acceso: Múltiples generadores (N threads)
```

**Problema sin sincronización**:

```
Generador 1: lee nextId=10
Generador 2: lee nextId=10  ← RACE CONDITION
Generador 1: nextId=11
Generador 2: nextId=11      ← IDs duplicados
```

**Solución con semáforo**:

```
Generador 1: wait(SEM_IDS) → nextId=10, nextId++ → signal(SEM_IDS)
Generador 2: wait(SEM_IDS) [bloqueado]
Generador 1: [libera]
Generador 2: → nextId=11, nextId++ → signal(SEM_IDS)
```

#### Sección Crítica 2: Buffer Compartido

```
Recurso Compartido: data->buffer
Semáforo: SEM_BUFFER
Acceso: N generadores (escritura) + 1 coordinador (lectura)
```

**Problema sin sincronización**:

```
Generador: strcat(buffer, "datos1")
Coordinador: strcpy(local, buffer)  ← LECTURA INCONSISTENTE
Generador: strcat(buffer, "datos2")
```

**Solución con semáforo**:

```
Generador: wait(SEM_BUFFER) → write → signal(SEM_BUFFER)
Coordinador: wait(SEM_BUFFER) → read & clear → signal(SEM_BUFFER)
```

#### Sección Crítica 3: Archivo CSV

```
Recurso Compartido: generated_data.csv
Semáforo: SEM_WRITE
Acceso: Solo coordinador (pero múltiples writes)
```

**Propósito**: Aunque solo el coordinador escribe, el semáforo garantiza:

- Escrituras atómicas
- Integridad del archivo
- Sincronización con `regEscritos`

### Patrones de Concurrencia Implementados

#### 1. Productor-Consumidor

```
PRODUCTORES (Generadores)     BUFFER (Memoria Compartida)     CONSUMIDOR (Coordinador)
      │                                │                              │
      ├─ produce datos ───────────────>│                              │
      │                                │<──── consume datos ──────────┤
      ├─ produce datos ───────────────>│                              │
      │                                │<──── consume datos ──────────┤
```

**Características**:

- N productores, 1 consumidor
- Buffer acotado (SHM_SIZE)
- Sin pérdida de datos
- Control de flujo implícito

#### 2. Pool de Workers

```
                COORDINADOR (Master)
                       │
        ┌──────────────┼──────────────┐
        │              │              │
    Worker 1       Worker 2  ...  Worker N
    (Gen 1)        (Gen 2)        (Gen N)
        │              │              │
        └──────────────┴──────────────┘
                 Shared Pool
              (IDs disponibles)
```

**Características**:

- IDs asignados dinámicamente
- Balance de carga automático
- Sin asignación estática

#### 3. Barrier Implícito

```c
// Proceso principal espera a todos los hijos
for (int i = 0; i < cantGen; i++) {
    waitpid(generador[i], &status, 0);
}
// Solo aquí se procede a cleanup
```

## Manejo de Señales

### Señales Capturadas

| Señal   | Valor | Trigger      | Acción                 |
| ------- | ----- | ------------ | ---------------------- |
| SIGINT  | 2     | Ctrl+C       | Terminación controlada |
| SIGTERM | 15    | kill         | Terminación controlada |
| SIGQUIT | 3     | Ctrl+\\      | Terminación + cleanup  |
| SIGCHLD | 17    | Hijo termina | Reaping de zombies     |

### Manejador de Señales

```c
void signal_handler(int sig)
{
    printf("\nSeñal %d recibida\n", sig);

    // 1. Marcar flag de shutdown
    if (data) {
        data->shutdown_flag = 1;
    }

    // 2. Limpiar recursos IPC
    cleanup_resources();

    // 3. Terminar proceso
    exit(0);
}
```

### Propagación de Shutdown

```
Usuario: Ctrl+C
    │
    ▼
Proceso Principal: signal_handler()
    │
    ├─ data->shutdown_flag = 1
    │
    ├──────────────────┬──────────────────┐
    │                  │                  │
    ▼                  ▼                  ▼
Generador 1      Generador 2      Coordinador
    │                  │                  │
while (!shutdown)  while (!shutdown)  while (!shutdown)
    │                  │                  │
    └─ exit(0)         └─ exit(0)         └─ exit(0)
```

## Gestión de Memoria

### Memoria Estática

```c
// En stack de cada proceso
char localBuffer[SHM_SIZE];
int ids[IDS_PER_REQUEST];
char record[RECORD_SIZE];
```

### Memoria Dinámica

```c
// Proceso principal
pid_t *generador = malloc(cantGen * sizeof(pid_t));

// Coordinador
int *regProcesados = malloc(sizeof(int) * data->total);
```

### Memoria Compartida

```c
// Segmento IPC compartido entre todos los procesos
sharedData *data = shmat(shm_id, NULL, 0);

// Liberación
shmdt(data);
shmctl(shm_id, IPC_RMID, NULL);
```

## Análisis de Rendimiento

### Factores que Afectan el Rendimiento

1. **Número de Generadores**

   - Más generadores → Más concurrencia
   - Demasiados → Contención en semáforos
   - Óptimo ≈ número de CPUs

2. **Tamaño del Buffer**

   - Buffer grande → Menos bloqueos
   - Buffer pequeño → Más contención
   - Trade-off: memoria vs. latencia

3. **IDs por Request**
   - Más IDs → Menos contención en SEM_IDS
   - Menos IDs → Mejor balance de carga
   - Configuración: `IDS_PER_REQUEST = 10`

### Métricas de Rendimiento

```bash
# Tiempo de ejecución
time ./bin/DataGenerator 4 10000

# Throughput
registros_generados / tiempo_total

# Utilización de CPU
top -p <pid>
```

### Cuello de Botella Típicos

1. **Contención en SEM_IDS**

   - Síntoma: Generadores esperan mucho por IDs
   - Solución: Aumentar `IDS_PER_REQUEST`

2. **Buffer Lleno**

   - Síntoma: Generadores reintentan writes
   - Solución: Aumentar `SHM_SIZE` o acelerar coordinador

3. **Escritura en CSV**
   - Síntoma: Coordinador es el cuello de botella
   - Solución: Buffer más grande en C, `fflush()` menos frecuente

## Diagrama de Estados de Proceso

```
┌─────────────┐
│   INICIO    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  GENERANDO  │◄──────┐
│   (activo)  │       │
└──────┬──────┘       │
       │              │
       ├─ IDs agotados│
       │              │
       ├─ Esperando   │
       │  semáforo    │
       │              │
       ├─ Buffer lleno│
       │  (reintento) ├─┘
       │
       ├─ shutdown=1
       │
       ▼
┌─────────────┐
│ TERMINANDO  │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   ZOMBIE    │
└──────┬──────┘
       │ wait()
       ▼
┌─────────────┐
│  TERMINADO  │
└─────────────┘
```

## Garantías del Sistema

### Propiedades de Seguridad (Safety)

✅ **Sin IDs duplicados**: Garantizado por sincronización de `nextId`

✅ **Sin escrituras concurrentes en CSV**: Garantizado por `SEM_WRITE`

✅ **Buffer sin corrupción**: Garantizado por `SEM_BUFFER`

✅ **Exactamente N registros**: Garantizado por contador `regEscritos`

### Propiedades de Vivacidad (Liveness)

✅ **Progreso garantizado**: Si hay IDs disponibles y buffer con espacio, los generadores progresan

✅ **Terminación**: Sistema termina cuando `regEscritos == total`

✅ **No deadlock**: Los semáforos se adquieren y liberan en orden consistente

✅ **No starvation**: Los procesos eventualmente acceden a recursos (semáforos son FIFO)

### Invariantes del Sistema

```
INVARIANTE 1: 1 <= nextId <= total + 1
INVARIANTE 2: 0 <= regEscritos <= total
INVARIANTE 3: 0 <= genActivos <= cantGen
INVARIANTE 4: strlen(buffer) < SHM_SIZE
INVARIANTE 5: Cada ID en [1, total] aparece exactamente 1 vez en CSV
```

## Limitaciones y Consideraciones

### Limitaciones Actuales

1. **Buffer Fijo**: Tamaño fijo de 25600 bytes
2. **Sin Prioridades**: Todos los generadores tienen igual prioridad
3. **CSV Simple**: No maneja caracteres especiales o comillas
4. **Sin Persistencia**: Shutdown abrupto puede perder datos en buffer

### Posibles Mejoras

1. **Buffer Circular**: Implementar cola circular para mejor uso de memoria
2. **Múltiples Coordinadores**: Particionar CSV para paralelizar escritura
3. **Compresión**: Comprimir datos en buffer para mayor throughput
4. **Checkpointing**: Guardar estado periódicamente para recuperación

## Debugging y Herramientas

### Compilar con Debug

```bash
# En Makefile agregar:
CFLAGS = -Wall -Wextra -g -DDEBUG

# Ejecutar con gdb
gdb ./bin/DataGenerator
(gdb) run 4 1000
```

### Ver Recursos IPC

```bash
# Memoria compartida
ipcs -m -p  # Ver procesos adjuntos

# Semáforos
ipcs -s -t  # Ver tiempos de acceso

# Todo junto
watch -n 1 'ipcs -a'
```

### Tracing de Llamadas

```bash
# strace para system calls
strace -f -e trace=ipc ./bin/DataGenerator 2 100

# ltrace para library calls
ltrace -f ./bin/DataGenerator 2 100
```

### Profiling

```bash
# Compilar con profiling
gcc -pg ...

# Ejecutar
./bin/DataGenerator 4 1000

# Analizar
gprof ./bin/DataGenerator gmon.out
```

---

**Autor**: Agustin  
**Materia**: Sistemas Operativos - UNLAM  
**Fecha**: Octubre 2025
