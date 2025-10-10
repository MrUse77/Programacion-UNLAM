# Resumen Ejecutivo del Proyecto

## Ejercicio 2: Cliente-Servidor de Micro Base de Datos con Transacciones

### Autor: Agustin

### Fecha: Octubre 2025

### Materia: Sistemas Operativos - UNLAM

---

## Descripción General

Sistema completo de base de datos distribuida implementado en C que permite múltiples clientes concurrentes conectarse a un servidor para realizar operaciones CRUD sobre un archivo CSV, con soporte completo de transacciones y control de concurrencia.

## Características Implementadas

### ✅ Requisitos Cumplidos

1. **Arquitectura Cliente-Servidor**

   - Comunicación mediante sockets TCP/IP
   - Servidor multi-threaded con POSIX threads
   - Cliente interactivo con prompt de comandos

2. **Concurrencia**

   - Soporte configurable para N clientes simultáneos
   - Cola de espera de M clientes
   - Un thread por cada cliente conectado
   - Sincronización mediante mutexes

3. **Operaciones de Base de Datos**

   - SELECT: Consultas con filtros
   - INSERT: Alta de registros
   - UPDATE: Modificación de registros
   - DELETE: Baja de registros

4. **Sistema de Transacciones**

   - BEGIN TRANSACTION: Inicio de transacción con lock exclusivo
   - COMMIT TRANSACTION: Confirmación de cambios
   - Bloqueo automático durante transacción
   - Liberación automática en caso de desconexión

5. **Manejo de Errores**

   - Desconexiones inesperadas (cliente y servidor)
   - Servidor lleno
   - Comandos inválidos
   - Transacciones bloqueadas

6. **Configuración Flexible**

   - Parámetros por línea de comandos
   - Validación completa de parámetros
   - Mensajes de ayuda descriptivos

7. **Limpieza de Recursos**
   - Cierre correcto de sockets
   - Destrucción de mutexes
   - Eliminación de archivos temporales
   - Manejadores de señales (SIGINT, SIGTERM)

## Archivos del Proyecto

### Código Fuente

- **server.c** (21 KB): Implementación del servidor
- **client.c** (5.7 KB): Implementación del cliente

### Compilación

- **Makefile** (2.2 KB): Sistema de compilación completo

### Documentación

- **README.md** (7.4 KB): Manual de usuario
- **TESTING.md** (7.3 KB): Guía de pruebas y monitoreo
- **ARCHITECTURE.md** (XX KB): Documentación técnica de arquitectura
- **RESUMEN.md** (este archivo): Resumen ejecutivo

### Scripts de Prueba

- **test_server.sh**: Iniciar servidor con verificaciones
- **test_concurrent.sh**: Pruebas automatizadas de concurrencia
- **monitor.sh**: Monitoreo en tiempo real del sistema

### Configuración

- **server.conf**: Archivo de configuración opcional
- **.gitignore**: Control de versiones

### Base de Datos

- **database.csv**: Archivo CSV de ejemplo

## Compilación y Uso

### Compilar

```bash
make
```

### Ejecutar Servidor

```bash
./server -p 8080 -n 5 -m 10
```

### Ejecutar Cliente

```bash
./client -s 127.0.0.1 -p 8080
```

### Pruebas Automatizadas

```bash
# Terminal 1
./test_server.sh

# Terminal 2
./test_concurrent.sh

# Terminal 3
./monitor.sh
```

## Protocolo de Comunicación

### Comandos Soportados

| Comando         | Sintaxis                | Ejemplo                                |
| --------------- | ----------------------- | -------------------------------------- |
| SELECT ALL      | `SELECT ALL`            | Mostrar todos los registros            |
| SELECT filtrado | `SELECT campo valor`    | `SELECT City Madrid`                   |
| INSERT          | `INSERT id,datos...`    | `INSERT 500,Juan,30,Madrid,IT,50000,5` |
| UPDATE          | `UPDATE id campo valor` | `UPDATE 500 Salary 55000`              |
| DELETE          | `DELETE id`             | `DELETE 500`                           |
| BEGIN           | `BEGIN TRANSACTION`     | Iniciar transacción                    |
| COMMIT          | `COMMIT TRANSACTION`    | Confirmar transacción                  |
| QUIT            | `QUIT`                  | Salir                                  |

### Formato de Respuestas

**Éxito:**

```
OK
[datos]
[mensaje]
```

**Error:**

```
ERROR: descripción del error
```

## Arquitectura Técnica

### Servidor

```
Main Thread
    │
    ├─► accept() nuevas conexiones
    ├─► Validar límite de clientes
    └─► Crear thread por cliente
         │
         ├─► Thread Cliente 1 ──┐
         ├─► Thread Cliente 2 ──┤
         └─► Thread Cliente N ──┤
                                 │
                            Mutex DB
                                 │
                           database.csv
```

### Control de Concurrencia

- **Mutex `db_mutex`**: Protege acceso al archivo CSV
- **Variable `transaction_owner`**: ID del cliente con transacción activa
- **Mutex `client_mutex`**: Protege contador de clientes

### Estados de Transacción

1. **Sin transacción**: `transaction_owner = -1`

   - Cualquier cliente puede hacer operaciones
   - Cada operación adquiere y libera el lock

2. **Con transacción**: `transaction_owner = client_id`
   - Solo el cliente propietario puede operar
   - Otros clientes reciben error
   - Lock se mantiene hasta COMMIT

## Pruebas Realizadas

### 1. Consultas Concurrentes

- ✅ Múltiples clientes haciendo SELECT simultáneamente
- ✅ Sin bloqueos ni conflictos

### 2. Transacciones

- ✅ BEGIN TRANSACTION adquiere lock exclusivo
- ✅ Otros clientes bloqueados correctamente
- ✅ COMMIT libera el lock
- ✅ Liberación automática si cliente se desconecta

### 3. Operaciones CRUD

- ✅ INSERT funciona correctamente
- ✅ UPDATE modifica registros
- ✅ DELETE elimina registros
- ✅ SELECT muestra cambios actualizados

### 4. Manejo de Errores

- ✅ Servidor rechaza conexiones cuando está lleno
- ✅ Desconexión abrupta de cliente no afecta al servidor
- ✅ Ctrl+C en servidor cierra todo ordenadamente
- ✅ Comandos inválidos devuelven errores descriptivos

### 5. Limpieza de Recursos

- ✅ No quedan procesos zombie
- ✅ No quedan sockets abiertos
- ✅ No quedan archivos temporales
- ✅ Mutexes destruidos correctamente

## Monitoreo en Linux

### Comandos Utilizados

```bash
# Procesos y threads
ps aux | grep server
ps -eLf | grep server

# Conexiones de red
netstat -tan | grep 8080
lsof -i :8080

# Archivos abiertos
lsof -p $(pgrep server)

# Recursos del sistema
htop -p $(pgrep server)

# Recursos IPC
ipcs -a
```

### Evidencias Capturadas

1. Socket del servidor en estado LISTEN
2. Múltiples conexiones ESTABLISHED
3. Threads creados para cada cliente
4. Archivo database.csv abierto en modo lectura/escritura
5. Uso de CPU y memoria por cliente
6. Limpieza completa al finalizar

## Decisiones de Diseño

### 1. Threads Detached

**Decisión**: Usar `PTHREAD_CREATE_DETACHED`
**Razón**: Limpieza automática sin necesidad de join()

### 2. Archivo Temporal para UPDATE/DELETE

**Decisión**: Crear `database.tmp` y reemplazar original
**Razón**: CSV no soporta modificación in-place eficiente

### 3. Lock Global vs. Lock por Registro

**Decisión**: Lock global en toda la base de datos
**Razón**: Simplicidad y atomicidad de transacciones

### 4. Protocolo de Texto Plano

**Decisión**: Comandos y respuestas en texto ASCII
**Razón**: Facilita debugging y testing manual

### 5. Cliente Interactivo

**Decisión**: Prompt estilo SQL
**Razón**: Familiar para usuarios y fácil de usar

## Posibles Mejoras Futuras

### Funcionalidad

- [ ] ROLLBACK para cancelar transacciones
- [ ] Múltiples tablas (varios CSV)
- [ ] Índices para búsquedas rápidas
- [ ] JOIN entre tablas
- [ ] WHERE con operadores (>, <, !=)

### Performance

- [ ] Pool de threads en lugar de crear/destruir
- [ ] Cache en memoria del CSV
- [ ] Compresión de datos
- [ ] Lectura mmap() para archivos grandes

### Seguridad

- [ ] Autenticación de usuarios
- [ ] SSL/TLS para comunicación cifrada
- [ ] Validación de inyección de comandos
- [ ] Límites de rate per cliente

### Confiabilidad

- [ ] Write-Ahead Log (WAL)
- [ ] Checkpoints periódicos
- [ ] Replicación master-slave
- [ ] Backup automático

## Conclusiones

### Objetivos Alcanzados

✅ Sistema completamente funcional de cliente-servidor
✅ Soporte completo para múltiples clientes concurrentes
✅ Sistema de transacciones con locks exclusivos
✅ Operaciones CRUD completas sobre CSV
✅ Manejo robusto de errores y desconexiones
✅ Limpieza correcta de recursos
✅ Documentación completa y detallada
✅ Scripts de prueba automatizados
✅ Guías de monitoreo con herramientas Linux

### Aprendizajes Clave

1. **Programación Concurrente**: Uso de threads POSIX y sincronización con mutexes
2. **Programación de Sockets**: TCP/IP con accept/connect/send/recv
3. **Gestión de Recursos**: Importancia de limpiar recursos del sistema
4. **Manejo de Señales**: Interceptar SIGINT/SIGTERM para cierre ordenado
5. **Protocolo de Comunicación**: Diseño de protocolos cliente-servidor
6. **Testing y Debugging**: Herramientas de monitoreo de Linux

### Cumplimiento de Criterios

| Criterio                 | Estado | Notas                             |
| ------------------------ | ------ | --------------------------------- |
| Validación de parámetros | ✅     | Con mensajes de ayuda             |
| Conexión TCP/IP          | ✅     | Funciona en red real              |
| Finalización controlada  | ✅     | Manejo de señales y desconexiones |
| Limpieza de recursos     | ✅     | Sin leaks de memoria/sockets      |
| Compilación con Makefile | ✅     | Con reglas clean/run              |
| Documentación            | ✅     | README, TESTING, ARCHITECTURE     |

## Estructura de Entrega

```
Ejercicio 2 TP/
├── server.c              # Código servidor
├── client.c              # Código cliente
├── Makefile              # Sistema de compilación
├── database.csv          # Base de datos de ejemplo
├── README.md             # Manual de usuario
├── TESTING.md            # Guía de pruebas
├── ARCHITECTURE.md       # Documentación técnica
├── RESUMEN.md            # Este archivo
├── test_server.sh        # Script de prueba servidor
├── test_concurrent.sh    # Script de prueba concurrencia
├── monitor.sh            # Script de monitoreo
├── server.conf           # Configuración opcional
└── .gitignore           # Control de versiones

Ejecutables (generados por make):
├── server                # Binario del servidor
└── client                # Binario del cliente
```

## Contacto y Referencias

**Estudiante**: Agustin
**Universidad**: UNLAM
**Materia**: Sistemas Operativos
**Fecha**: Octubre 2025

**Referencias utilizadas**:

- POSIX Threads Programming (pthread)
- Berkeley Sockets API
- Linux System Programming
- Stevens, W. Richard - "Unix Network Programming"
- man pages: socket(2), pthread(3), mutex(3)

---

## Anexo: Comandos de Verificación Rápida

```bash
# Compilar y verificar
make clean && make

# Iniciar servidor
./server -p 8080 -n 3 -m 5 &

# Ver que el servidor está escuchando
netstat -tlnp | grep 8080

# Conectar cliente y probar
echo "SELECT ALL" | ./client -s 127.0.0.1 -p 8080

# Ver threads del servidor
ps -eLf | grep server

# Limpiar
pkill server
make clean
```

---

**Este proyecto demuestra conocimientos prácticos de:**

- Programación en C
- Programación concurrente (threads)
- Programación de redes (sockets)
- Sistemas operativos Linux
- Sincronización y control de concurrencia
- Manejo de recursos del sistema
- Testing y debugging
- Documentación técnica

✅ **Proyecto completado y listo para entrega**
