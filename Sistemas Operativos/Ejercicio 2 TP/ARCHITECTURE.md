# Documentación Técnica - Arquitectura y Protocolo

## Arquitectura del Sistema

### Modelo Cliente-Servidor

```markdown
┌─────────────────────────────────────────────────────────────┐
│                         SERVIDOR                            │
│                                                             │
│  ┌────────────────────────────────────────────────────┐     │
│  │  Thread Principal                                  │     │
│  │  - accept() nuevas conexiones                      │     │
│  │  - Verificar límite de clientes                    │     │
│  │  - Crear thread por cliente                        │     │
│  └────────────────────────────────────────────────────┘     │
│                         │                                   │
│       ┌─────────────────┼─────────────────┐                 │
│       │                 │                 │                 │
│  ┌────▼─────┐     ┌────▼─────┐     ┌────▼─────┐             │
│  │ Thread 1 │     │ Thread 2 │ ... │ Thread N │             │
│  │ Cliente1 │     │ Cliente2 │     │ ClienteN │             │
│  └────┬─────┘     └────┬─────┘     └────┬─────┘             │
│       │                │                │                   │
│       └────────────────┼────────────────┘                   │
│                        │                                    │
│              ┌─────────▼─────────┐                          │
│              │   DB Mutex Lock   │                          │
│              │  Control Acceso   │                          │
│              └─────────┬─────────┘                          │
│                        │                                    │
│                 ┌──────▼──────┐                             │
│                 │ database.csv│                             │
│                 └─────────────┘                             │
└─────────────────────────────────────────────────────────────┘

┌──────────┐        ┌──────────┐        ┌──────────┐
│ Cliente 1│        │ Cliente 2│        │ Cliente N│
│  (TCP)   │        │  (TCP)   │        │  (TCP)   │
└──────────┘        └──────────┘        └──────────┘
```

### Componentes del Servidor

#### 1. Thread Principal

- **Responsabilidad**: Aceptar nuevas conexiones
- **Función**: `main()`
- **Tareas**:
  - Inicializar socket
  - Configurar bind() y listen()
  - Loop infinito de accept()
  - Validar límite de clientes
  - Crear thread detached por cliente

#### 2. Threads de Cliente

- **Responsabilidad**: Atender solicitudes de un cliente
- **Función**: `handle_client()`
- **Tareas**:
  - Recibir comandos del cliente
  - Procesar comandos
  - Enviar respuestas
  - Manejar desconexiones
  - Liberar recursos

#### 3. Gestor de Base de Datos

- **Responsabilidad**: Ejecutar operaciones en el CSV
- **Funciones**:
  - `execute_query()`: SELECT
  - `execute_insert()`: INSERT
  - `execute_update()`: UPDATE
  - `execute_delete()`: DELETE
- **Sincronización**: Mutex para acceso al archivo

#### 4. Gestor de Transacciones

- **Responsabilidad**: Control de locks exclusivos
- **Variables**:
  - `transaction_owner`: ID del cliente con transacción activa
  - `db_mutex`: Mutex para sincronización
- **Estados**:
  - Sin transacción: `transaction_owner = -1`
  - Con transacción: `transaction_owner = client_id`

### Componentes del Cliente

```Markdown
┌─────────────────────────────────┐
│           CLIENTE               │
│                                 │
│  ┌──────────────────────┐       │
│  │  Interface Usuario   │       │
│  │  (Prompt interactivo)│       │
│  └──────────┬───────────┘       │
│             │                   │
│  ┌──────────▼───────────┐       │
│  │  Procesador Comandos │       │
│  │  - Validación local  │       │
│  │  - HELP local        │       │
│  └──────────┬───────────┘       │
│             │                   │
│  ┌──────────▼───────────┐       │
│  │   Socket TCP/IP      │       │
│  │   - send()           │       │
│  │   - recv()           │       │
│  └──────────────────────┘       │
└─────────────────────────────────┘
```

## Protocolo de Comunicación

### Formato de Mensajes

#### Comandos Cliente → Servidor

Formato: `COMANDO [PARAMETROS]\n`

**SELECT:**

```mar
SELECT ALL\n
SELECT campo valor\n
```

**INSERT:**

```mar
INSERT id,campo1,campo2,...,campoN\n
```

**UPDATE:**

```mar
UPDATE id campo nuevo_valor\n
```

**DELETE:**

```mar
DELETE id\n
```

**TRANSACCIONES:**

```mar
BEGIN TRANSACTION\n
COMMIT TRANSACTION\n
```

**CONTROL:**

```mar
QUIT\n
```

#### Respuestas Servidor → Cliente

Formato:

```mar
STATUS\n
[DATOS]\n
[MENSAJE]\n
```

**Respuesta Exitosa:**

```mar
OK\n
[datos de resultado]\n
[mensaje informativo]\n
```

**Respuesta Error:**

```mar
ERROR: descripción del error\n
```

### Ejemplos de Intercambio

#### Ejemplo 1: SELECT exitoso

```mar
Cliente → Servidor:
SELECT City Madrid\n

Servidor → Cliente:
OK\n
ID,Name,Age,City,Department,Salary,Experience\n
5,Pedro,60,Madrid,Marketing,66362,41\n
Registros encontrados: 1\n
```

#### Ejemplo 2: INSERT exitoso

```mar
Cliente → Servidor:
INSERT 500,Juan,30,Madrid,IT,50000,5\n

Servidor → Cliente:
OK\n
Registro insertado correctamente\n
```

#### Ejemplo 3: UPDATE exitoso

```markdown
Cliente → Servidor:
UPDATE 500 Salary 55000\n

Servidor → Cliente:
OK\n
Registro actualizado correctamente\n
```

#### Ejemplo 4: Transacción bloqueada

```markdown
Cliente 2 → Servidor:
SELECT ALL\n

Servidor → Cliente 2:
ERROR: Existe una transacción activa. Reintente más tarde.\n
```

## Gestión de Concurrencia

### Mutex para Base de Datos

```c
pthread_mutex_t db_mutex;

// Inicialización
pthread_mutex_init(&db_mutex, NULL);

// Uso en consulta sin transacción
pthread_mutex_lock(&db_mutex);
execute_query(command, response);
pthread_mutex_unlock(&db_mutex);

// Destrucción
pthread_mutex_destroy(&db_mutex);
```

### Control de Transacciones

**Estado de la transacción:**

```c
static int transaction_owner = -1;  // -1 = sin transacción
                                     // >= 0 = ID del cliente propietario
```

**BEGIN TRANSACTION:**

```c
pthread_mutex_lock(&db_mutex);
if (transaction_owner != -1 && transaction_owner != client_id) {
    // Ya hay transacción activa de otro cliente
    pthread_mutex_unlock(&db_mutex);
    return ERROR;
}
transaction_owner = client_id;
pthread_mutex_unlock(&db_mutex);
// El cliente ahora tiene el lock exclusivo
```

**COMMIT TRANSACTION:**

```c
pthread_mutex_lock(&db_mutex);
if (transaction_owner != client_id) {
    // El cliente no tiene transacción activa
    pthread_mutex_unlock(&db_mutex);
    return ERROR;
}
transaction_owner = -1;  // Liberar transacción
pthread_mutex_unlock(&db_mutex);
```

**Operación con transacción activa:**

```c
pthread_mutex_lock(&db_mutex);
if (transaction_owner != -1 && transaction_owner != client_id) {
    // Otro cliente tiene transacción activa
    pthread_mutex_unlock(&db_mutex);
    return ERROR_TRANSACTION_ACTIVE;
}

// Si este cliente tiene la transacción, mantener el lock
int has_transaction = (transaction_owner == client_id);
if (!has_transaction) {
    pthread_mutex_unlock(&db_mutex);
}

// Realizar operación...

if (has_transaction) {
    pthread_mutex_unlock(&db_mutex);
}
```

## Manejo de Conexiones

### Límite de Clientes

```c
static int client_count = 0;
static int max_clients = 5;
pthread_mutex_t client_mutex;

// En accept():
pthread_mutex_lock(&client_mutex);
if (client_count >= max_clients) {
    pthread_mutex_unlock(&client_mutex);
    send_error("Servidor lleno");
    close(client_socket);
    continue;
}
client_count++;
pthread_mutex_unlock(&client_mutex);

// Al desconectar:
pthread_mutex_lock(&client_mutex);
client_count--;
pthread_mutex_unlock(&client_mutex);
```

### Cola de Espera

```c
// En listen()
listen(server_socket, max_queue);
```

- `max_queue`: Clientes en espera mientras se aceptan otros
- Si se supera, nuevas conexiones reciben ECONNREFUSED

## Manejo de Errores y Desconexiones

### Desconexión Normal del Cliente

```c
// Cliente envía QUIT
if (strcmp(command, "QUIT") == 0) {
    send_response("OK\nDesconectando...\n");
    close(client_socket);
    free(client);
    return;
}
```

### Desconexión Inesperada del Cliente

```c
int bytes = recv(client_socket, buffer, BUFFER_SIZE, 0);
if (bytes <= 0) {
    if (bytes == 0) {
        // Cliente cerró conexión
        printf("Cliente desconectado\n");
    } else {
        // Error de red
        perror("Error al recibir");
    }

    // Liberar transacción si la tenía
    pthread_mutex_lock(&db_mutex);
    if (transaction_owner == client_id) {
        transaction_owner = -1;
    }
    pthread_mutex_unlock(&db_mutex);

    close(client_socket);
    free(client);
    return;
}
```

### Señales del Sistema

```c
void signal_handler(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        cleanup_resources();
        exit(0);
    }
}

// En main():
signal(SIGINT, signal_handler);
signal(SIGTERM, signal_handler);
signal(SIGPIPE, SIG_IGN);  // Ignorar SIGPIPE
```

## Operaciones sobre CSV

### Estructura del Archivo

```markdown
ID,Name,Age,City,Department,Salary,Experience
1,Carmen,45,Malaga,Finance,64665,6
2,Luis,32,Malaga,HR,95307,0
...
```

### SELECT: Lectura Directa

```c
FILE* file = fopen(DB_FILE, "r");
// Leer línea por línea
// Filtrar según criterios
// Devolver resultados
fclose(file);
```

### INSERT: Append

```c
FILE* file = fopen(DB_FILE, "a");
fprintf(file, "%s\n", new_record);
fclose(file);
```

### UPDATE y DELETE: Archivo Temporal

```c
FILE* file = fopen(DB_FILE, "r");
FILE* temp = fopen(TEMP_FILE, "w");

// Copiar encabezado
// Procesar cada línea:
//   - UPDATE: modificar la línea correspondiente
//   - DELETE: omitir la línea correspondiente

fclose(file);
fclose(temp);

// Reemplazar archivo original
remove(DB_FILE);
rename(TEMP_FILE, DB_FILE);
```

## Optimizaciones y Consideraciones

### Performance

1. **Buffers**: Tamaño 4KB para comunicación de red
2. **Threads Detached**: No requieren join(), se limpian automáticamente
3. **SO_REUSEADDR**: Permite reusar el puerto inmediatamente
4. **Archivos Temporales**: Solo para UPDATE/DELETE

### Seguridad

1. **Validación de Parámetros**: En servidor y cliente
2. **Límites de Buffers**: Prevenir overflow
3. **Manejo de Strings**: Uso de strncpy, snprintf
4. **Control de Acceso**: Mutex para sincronización

### Escalabilidad

**Límites actuales:**

- Clientes concurrentes: Configurable (default 5)
- Cola de espera: Configurable (default 10)
- Tamaño de mensaje: 4KB
- Tamaño de línea CSV: 1KB

**Para mejorar:**

- Pool de threads en lugar de crear/destruir
- Cache en memoria del CSV
- Índices para búsquedas rápidas
- Transacciones con ROLLBACK
- Log de operaciones (WAL)

### Portabilidad

**Linux específico:**

- POSIX threads (pthread)
- Berkeley sockets
- Signal handling POSIX

**Para Windows:**

- Reemplazar pthread con Windows threads
- Reemplazar sockets con Winsock2
- Adaptar signal handling

## Diagramas de Flujo

### Flujo del Servidor

```markdown
┌─────────────────┐
│   Iniciar       │
│   Servidor      │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Parsear Args    │
│ Validar Params  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Crear Socket    │
│ bind(), listen()│
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Loop: accept()  │◄─────────┐
└────────┬────────┘          │
         │                   │
         ▼                   │
┌─────────────────┐          │
│¿Límite clientes?│──NO──┐   │
└────────┬────────┘      │   │
         │YES            │   │
         ▼               ▼   │
┌─────────────────┐ ┌────────────┐
│ Rechazar        │ │ Crear      │
│ Conexión        │ │ Thread     │
└────────┬────────┘ └─────┬──────┘
         │                │
         │                ▼
         │          ┌────────────┐
         │          │ Atender    │
         │          │ Cliente    │
         │          └─────┬──────┘
         │                │
         └────────────────┴───────┘
```

### Flujo de Transacción

```markdown
Cliente 1                    Servidor                   Cliente 2

    │                           │                           │
    │  BEGIN TRANSACTION        │                           │
    ├──────────────────────────►│                           │
    │                           │                           │
    │      OK                   │                           │
    │◄──────────────────────────┤                           │
    │                           │                           │
    │                      [Lock Adquirido]                 │
    │                           │                           │
    │  UPDATE 1 Salary 70000    │                           │
    ├──────────────────────────►│                           │
    │                           │        SELECT ALL         │
    │      OK                   │◄──────────────────────────┤
    │◄──────────────────────────┤                           │
    │                           │                           │
    │  INSERT 500,...           │  ERROR: Transacción activa│
    ├──────────────────────────►├──────────────────────────►│
    │                           │                           │
    │      OK                   │                           │
    │◄──────────────────────────┤                           │
    │                           │                           │
    │  COMMIT TRANSACTION       │                           │
    ├──────────────────────────►│                           │
    │                           │                           │
    │      OK                   │                           │
    │◄──────────────────────────┤                           │
    │                           │                           │
    │                     [Lock Liberado]                   │
    │                           │                           │
    │                           │        SELECT ALL         │
    │                           │◄──────────────────────────┤
    │                           │                           │
    │                           │           OK              │
    │                           ├──────────────────────────►│
    │                           │                           │
```

## Métricas de Rendimiento Esperadas

### Latencia

- Operación SELECT local: < 10ms
- Operación INSERT: < 5ms
- Operación UPDATE/DELETE: < 50ms (requiere reescritura)

### Throughput

- Consultas concurrentes: ~100 ops/seg
- Con transacción: ~20 ops/seg (serializado)

### Recursos

- Memoria por thread: ~8MB
- CPU por cliente activo: ~1-5%
- I/O disco: Solo en modificaciones

Estas métricas dependen del hardware y del tamaño del archivo CSV.
