# Sistema Cliente-Servidor de Micro Base de Datos con Transacciones

Sistema de base de datos distribuida que utiliza sockets TCP/IP para permitir consultas y modificaciones remotas sobre un archivo CSV, con soporte para transacciones y control de concurrencia.

## Características

- **Arquitectura Cliente-Servidor**: Comunicación mediante sockets TCP/IP
- **Concurrencia**: Soporte para múltiples clientes simultáneos usando threads POSIX
- **Transacciones**: Sistema de transacciones con bloqueo exclusivo (BEGIN/COMMIT)
- **Operaciones CRUD**: SELECT, INSERT, UPDATE, DELETE
- **Control de acceso**: Gestión de locks durante transacciones
- **Manejo de errores**: Respuestas controladas ante desconexiones inesperadas
- **Configuración flexible**: Parámetros por línea de comandos

## Compilación

```bash
# Compilar todo (servidor y cliente)
make

# Compilar solo el servidor
make server

# Compilar solo el cliente
make client

# Ver ayuda de compilación
make help
```

## Uso

### Servidor

```bash
./server -p <puerto> [-n <max_clientes>] [-m <max_cola>]
```

**Parámetros:**

- `-p <puerto>`: Puerto de escucha (requerido)
- `-n <max_clientes>`: Máximo de clientes concurrentes (default: 5)
- `-m <max_cola>`: Máximo de clientes en espera (default: 10)
- `-h`: Mostrar ayuda

**Ejemplo:**

```bash
./server -p 8080 -n 10 -m 20
```

### Cliente

```bash
./client -s <servidor> -p <puerto>
```

**Parámetros:**

- `-s <servidor>`: Dirección IP o hostname del servidor (requerido)
- `-p <puerto>`: Puerto del servidor (requerido)
- `-h`: Mostrar ayuda

**Ejemplos:**

```bash
# Conectar a servidor local
./client -s 127.0.0.1 -p 8080

# Conectar a servidor remoto
./client -s 192.168.1.100 -p 8080
```

## Comandos del Cliente

### Consultas (SELECT)

```sql
-- Mostrar todos los registros
SELECT ALL

-- Buscar por campo específico
SELECT Name Carlos
SELECT City Madrid
SELECT Department IT
```

### Inserción (INSERT)

```sql
-- Formato: INSERT id,nombre,edad,ciudad,departamento,salario,experiencia
INSERT 500,Juan,30,Madrid,IT,50000,5
INSERT 501,Maria,28,Barcelona,Sales,45000,3
```

### Actualización (UPDATE)

```sql
-- Formato: UPDATE id campo nuevo_valor
UPDATE 500 Salary 55000
UPDATE 500 City Barcelona
UPDATE 500 Name JuanCarlos
```

### Eliminación (DELETE)

```sql
-- Formato: DELETE id
DELETE 500
```

### Transacciones

```sql
-- Iniciar transacción (obtiene bloqueo exclusivo)
BEGIN TRANSACTION

-- Realizar múltiples operaciones
UPDATE 1 Salary 70000
INSERT 600,Pedro,35,Valencia,HR,48000,7
DELETE 400

-- Confirmar transacción (libera bloqueo)
COMMIT TRANSACTION
```

**Comportamiento durante transacciones:**

- El cliente que inicia la transacción obtiene bloqueo exclusivo sobre la base de datos
- Otros clientes recibirán error si intentan consultar o modificar datos
- El bloqueo se mantiene hasta ejecutar COMMIT TRANSACTION
- Si el cliente se desconecta, el bloqueo se libera automáticamente

### Otros Comandos

```sql
HELP  -- Mostrar ayuda de comandos
QUIT  -- Salir del cliente
```

## Protocolo de Comunicación

### Formato de Mensajes

**Cliente → Servidor:**

- Comandos en texto plano seguidos de newline
- Ejemplos: `SELECT ALL\n`, `BEGIN TRANSACTION\n`

**Servidor → Cliente:**

- Respuestas en formato:
  ```
  OK|ERROR
  [datos]
  [mensaje informativo]
  ```

### Códigos de Respuesta

- `OK`: Operación exitosa
- `ERROR`: Operación fallida (incluye descripción del error)

## Estructura del Proyecto

```
.
├── server.c          # Código fuente del servidor
├── client.c          # Código fuente del cliente
├── Makefile          # Sistema de compilación
├── database.csv      # Base de datos CSV
└── README.md         # Este archivo
```

## Estructura del CSV

```csv
ID,Name,Age,City,Department,Salary,Experience
1,Carmen,45,Malaga,Finance,64665,6
2,Luis,32,Malaga,HR,95307,0
...
```

## Monitoreo en Linux

### Ver conexiones activas

```bash
# Ver sockets en escucha y conexiones establecidas
netstat -tlnp | grep server
netstat -tan | grep 8080

# Ver archivos abiertos por el servidor
lsof -p $(pgrep server)
lsof -i :8080

# Ver procesos y threads del servidor
ps aux | grep server
ps -eLf | grep server

# Monitoreo interactivo
htop -p $(pgrep server)
```

### Ejemplo de sesión de prueba

**Terminal 1 - Servidor:**

```bash
$ ./server -p 8080 -n 3 -m 5
Servidor iniciado en puerto 8080
Máximo de clientes concurrentes: 3
Máximo de clientes en espera: 5
Esperando conexiones...
```

**Terminal 2 - Cliente 1:**

```bash
$ ./client -s 127.0.0.1 -p 8080
Conectado al servidor!
db> BEGIN TRANSACTION
OK
Transacción iniciada

db> UPDATE 1 Salary 75000
OK
Registro actualizado correctamente
```

**Terminal 3 - Cliente 2:**

```bash
$ ./client -s 127.0.0.1 -p 8080
Conectado al servidor!
db> SELECT ALL
ERROR: Existe una transacción activa. Reintente más tarde.
```

**Terminal 2 - Cliente 1:**

```bash
db> COMMIT TRANSACTION
OK
Transacción confirmada
```

**Terminal 3 - Cliente 2:**

```bash
db> SELECT ALL
OK
ID,Name,Age,City,Department,Salary,Experience
1,Carmen,45,Malaga,Finance,75000,6
...
```

### Monitorear concurrencia

```bash
# En otra terminal mientras el servidor está corriendo
watch -n 1 'netstat -an | grep 8080 | grep ESTABLISHED | wc -l'

# Ver threads del servidor
watch -n 1 'ps -eLf | grep server | grep -v grep'
```

## Manejo de Errores

### Desconexión inesperada del cliente

- El servidor detecta la desconexión
- Si el cliente tenía transacción activa, se libera el bloqueo automáticamente
- Los recursos del cliente se liberan correctamente

### Desconexión del servidor

- Los clientes detectan la desconexión
- Se cierran los sockets de forma controlada
- Se muestra mensaje informativo al usuario

### Servidor lleno

- Si se alcanza el máximo de clientes concurrentes
- Nuevos clientes reciben mensaje de error
- La conexión se rechaza inmediatamente

## Limpieza de Recursos

### Manual

```bash
# Limpiar archivos compilados
make clean

# Limpiar todo incluyendo backups
make distclean

# Verificar que no quedan recursos abiertos
lsof | grep database
ipcs -a
```

### Automática

- Al cerrar el servidor con Ctrl+C, se ejecuta limpieza automática
- Los mutexes se destruyen correctamente
- Los archivos temporales se eliminan
- Los sockets se cierran

## Notas Importantes

1. **Archivo database.csv**: Debe existir en el directorio de ejecución del servidor
2. **Permisos**: Asegúrese de tener permisos de lectura/escritura sobre database.csv
3. **Puerto**: Si el puerto está en uso, elija otro o espere a que se libere
4. **Firewall**: Asegúrese de que el puerto elegido esté abierto en el firewall
5. **Formato CSV**: Mantenga el formato consistente (7 campos por registro)

## Solución de Problemas

### Error: "Address already in use"

```bash
# Esperar unos segundos y reintentar, o usar otro puerto
# O matar el proceso que usa el puerto:
lsof -ti:8080 | xargs kill -9
```

### Error: "Connection refused"

```bash
# Verificar que el servidor está corriendo
ps aux | grep server

# Verificar que el puerto es correcto
netstat -tlnp | grep server
```

### Error: "No se encuentra el archivo database.csv"

```bash
# Asegurarse de ejecutar el servidor desde el directorio correcto
# O crear un archivo database.csv con el formato adecuado
```

## Licencia

Este proyecto es parte de un trabajo práctico académico para Sistemas Operativos - UNLAM.
