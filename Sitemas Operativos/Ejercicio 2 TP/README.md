# Sistema Cliente-Servidor de Micro Base de Datos

## Descripción

Este proyecto implementa un sistema cliente-servidor que utiliza sockets TCP/IP para permitir consultas y modificaciones remotas sobre un archivo CSV que actúa como base de datos. El sistema soporta múltiples clientes concurrentes y transacciones con bloqueo exclusivo.

## Características Principales

### 🚀 Funcionalidades del Servidor
- **Múltiples clientes concurrentes**: Hasta N clientes simultáneos con M en cola de espera (configurable)
- **Base de datos CSV**: Utiliza archivos CSV como almacenamiento de datos
- **Transacciones ACID**: Sistema completo de transacciones con BEGIN/COMMIT
- **Bloqueo exclusivo**: Durante transacciones, solo el cliente activo puede modificar datos
- **Protocolo personalizado**: Comunicación eficiente entre cliente y servidor
- **Manejo robusto de errores**: Recuperación ante desconexiones inesperadas
- **Configuración flexible**: Parámetros configurables via archivo o línea de comandos

### 📱 Funcionalidades del Cliente
- **Interfaz interactiva**: Cliente con soporte para readline y historial de comandos
- **Comandos SQL**: SELECT, INSERT, UPDATE, DELETE con sintaxis familiar
- **Control de transacciones**: Comandos BEGIN TRANSACTION y COMMIT TRANSACTION
- **Indicadores visuales**: Prompt que indica estado de conexión y transacciones
- **Comandos especiales**: Comandos internos para estado y configuración

### 🔒 Sistema de Transacciones
- Cliente puede iniciar transacción con `BEGIN TRANSACTION`
- Obtiene bloqueo exclusivo sobre la base de datos
- Puede realizar múltiples modificaciones
- Confirma con `COMMIT TRANSACTION`
- Otros clientes reciben error de "base de datos bloqueada"

## Instalación y Compilación

### Requisitos del Sistema
```bash
# Ubuntu/Debian
sudo apt-get install gcc libc6-dev libreadline-dev net-tools lsof procps

# CentOS/RHEL/Fedora
sudo yum install gcc glibc-devel readline-devel net-tools lsof procps
```

### Compilación
```bash
# Compilar todo el proyecto
make

# Verificar dependencias
make check-deps

# Instalar dependencias automáticamente (Ubuntu/Debian)
make install-deps

# Compilar en modo debug
make debug

# Compilar optimizado para producción
make release

# Ejecutar tests básicos
make test
```

### Instalación del Sistema
```bash
# Instalar en el sistema
sudo make install

# Desinstalar
sudo make uninstall
```

## Configuración

### Archivo de Configuración (`config.txt`)
```ini
# Configuración del Servidor de Micro Base de Datos
HOST=127.0.0.1
PORT=8080
MAX_CLIENTS=5
QUEUE_SIZE=10
DATABASE_FILE=database.csv
LOG_LEVEL=INFO
```

### Base de Datos CSV (`database.csv`)
```csv
id,nombre,apellido,edad,departamento,salario
1,Juan,Perez,28,Ventas,45000
2,Maria,Garcia,32,Marketing,52000
3,Carlos,Lopez,25,IT,48000
```

## Uso del Sistema

### Iniciar el Servidor
```bash
# Usando configuración por defecto
./bin/microdb_server

# Con archivo de configuración personalizado
./bin/microdb_server mi_config.txt

# Usando make
make run-server
```

### Conectar Cliente
```bash
# Conectar a localhost:8080 (por defecto)
./bin/microdb_client

# Conectar a servidor específico
./bin/microdb_client -h 192.168.1.100 -p 8080

# Mostrar ayuda
./bin/microdb_client --help

# Usando make
make run-client
```

## Comandos SQL Soportados

### Consultas (SELECT)
```sql
-- Seleccionar todos los registros
SELECT

-- Seleccionar campos específicos
SELECT nombre,apellido

-- Consulta con condiciones
SELECT WHERE edad > 25

-- Consulta con condiciones complejas
SELECT nombre,salario WHERE departamento = 'IT'
```

### Inserción (INSERT)
```sql
-- Insertar nuevo registro (ID se asigna automáticamente)
INSERT VALUES ('Pedro','Martinez',29,'Ventas',44000)

-- Con comillas para valores con espacios
INSERT VALUES ('Ana Maria','Rodriguez Gomez',35,'Recursos Humanos',50000)
```

### Actualización (UPDATE)
```sql
-- Actualizar un campo
UPDATE SET salario=50000 WHERE id=1

-- Actualizar múltiples campos
UPDATE SET departamento='IT',salario=55000 WHERE nombre='Juan'

-- Actualizar con condiciones
UPDATE SET salario=salario+5000 WHERE departamento='Ventas'
```

### Eliminación (DELETE)
```sql
-- Eliminar registro específico
DELETE WHERE id=1

-- Eliminar por condición
DELETE WHERE departamento='Marketing'

-- Eliminar múltiples registros
DELETE WHERE edad < 25
```

### Transacciones
```sql
-- Iniciar transacción
BEGIN TRANSACTION

-- Realizar múltiples operaciones...
INSERT VALUES ('Nuevo','Usuario',30,'IT',60000)
UPDATE SET salario=salario*1.1 WHERE departamento='IT'
DELETE WHERE edad > 60

-- Confirmar cambios
COMMIT TRANSACTION
```

### Comandos de Información
```sql
-- Mostrar estructura de la tabla
DESCRIBE

-- Mostrar ayuda
HELP

-- Salir del cliente
QUIT
```

## Comandos Especiales del Cliente

```bash
# Estado de la conexión
.status

# Limpiar pantalla
.clear

# Ayuda del cliente
.help
```

## Protocolo de Comunicación

### Formato de Comandos
```
TIPO|QUERY|NUM_PARAMS|PARAM1|PARAM2|...\n
```

### Formato de Respuestas
```
CODIGO|MENSAJE|NUM_FILAS|DATOS\n
```

### Códigos de Respuesta
- `200`: OK - Operación exitosa
- `400`: Error - Error en el comando o datos
- `404`: No encontrado - Registro no existe
- `409`: Conflicto - Error de concurrencia
- `423`: Bloqueado - Base de datos en transacción
- `500`: Error del servidor - Error interno

## Monitoreo en Linux

### Verificar Procesos del Servidor
```bash
# Procesos del servidor
ps aux | grep microdb_server

# Threads del servidor
ps -eLf | grep microdb_server

# Uso de recursos
htop -p $(pgrep microdb_server)
```

### Verificar Conexiones de Red
```bash
# Sockets en escucha
netstat -tlnp | grep :8080

# Conexiones activas
netstat -anp | grep microdb

# Archivos abiertos por el servidor
lsof -p $(pgrep microdb_server)

# Puertos utilizados
lsof -i :8080
```

### Verificar Concurrencia
```bash
# Número de conexiones activas
netstat -an | grep :8080 | grep ESTABLISHED | wc -l

# Clientes conectados
netstat -anp | grep :8080 | grep ESTABLISHED
```

### Logs del Sistema
```bash
# Ver logs del servidor (si se usa syslog)
tail -f /var/log/syslog | grep microdb

# Monitorear archivo de base de datos
watch -n 2 'wc -l database.csv'

# Verificar bloqueos de archivos
lsof database.csv
```

## Estructura del Proyecto

```
microdb_server/
├── README.md              # Esta documentación
├── Makefile              # Reglas de compilación
├── config.txt            # Configuración del servidor
├── database.csv          # Base de datos de ejemplo
├── microdb.h            # Definiciones principales
├── protocol.h           # Protocolo de comunicación
├── database.h           # Funciones de base de datos
├── server.c             # Servidor principal
├── client.c             # Cliente interactivo
├── protocol.c           # Implementación del protocolo
├── database.c           # Manejo de base de datos
├── bin/                 # Ejecutables compilados
│   ├── microdb_server
│   └── microdb_client
└── obj/                 # Archivos objeto
    ├── server.o
    ├── client.o
    ├── protocol.o
    └── database.o
```

## Ejemplos de Uso Completos

### Ejemplo 1: Operaciones Básicas
```bash
# Terminal 1: Iniciar servidor
./bin/microdb_server

# Terminal 2: Conectar cliente
./bin/microdb_client
microdb> SELECT
microdb> INSERT VALUES ('Luis','Gonzalez',31,'Finanzas',52000)
microdb> UPDATE SET salario=55000 WHERE nombre='Luis'
microdb> DELETE WHERE id=1
microdb> QUIT
```

### Ejemplo 2: Transacciones
```bash
./bin/microdb_client
microdb> BEGIN TRANSACTION
microdb [TX]> INSERT VALUES ('Test','User',25,'IT',45000)
microdb [TX]> UPDATE SET salario=salario*1.1 WHERE departamento='IT'
microdb [TX]> COMMIT TRANSACTION
microdb> QUIT
```

### Ejemplo 3: Múltiples Clientes
```bash
# Cliente 1
./bin/microdb_client
microdb> BEGIN TRANSACTION
microdb [TX]> UPDATE SET salario=60000 WHERE id=2
# No hacer COMMIT aún...

# Cliente 2 (en otra terminal)
./bin/microdb_client
microdb> SELECT WHERE id=2
# Recibirá error: "Base de datos bloqueada por transacción activa"

# Cliente 1: Confirmar transacción
microdb [TX]> COMMIT TRANSACTION

# Cliente 2: Ahora puede acceder
microdb> SELECT WHERE id=2
```

## Troubleshooting

### Problemas Comunes

#### Servidor no inicia
```bash
# Verificar que el puerto no esté ocupado
netstat -tlnp | grep :8080

# Verificar permisos del archivo de base de datos
ls -la database.csv

# Verificar sintaxis del archivo de configuración
cat config.txt
```

#### Cliente no se conecta
```bash
# Verificar que el servidor esté corriendo
ps aux | grep microdb_server

# Probar conectividad
telnet localhost 8080

# Verificar firewall
sudo ufw status
```

#### Error "Permission denied"
```bash
# Dar permisos de ejecución
chmod +x bin/microdb_server bin/microdb_client

# Verificar propietario de archivos
ls -la database.csv config.txt
```

#### Error "Address already in use"
```bash
# Encontrar proceso usando el puerto
lsof -i :8080

# Terminar proceso anterior
kill -9 $(lsof -t -i:8080)

# O cambiar puerto en config.txt
echo "PORT=8081" >> config.txt
```

### Debug y Desarrollo

#### Compilar en modo debug
```bash
make debug
gdb ./bin/microdb_server
```

#### Verificar memoria con Valgrind
```bash
valgrind --leak-check=full ./bin/microdb_server
```

#### Monitorear llamadas de sistema
```bash
strace -p $(pgrep microdb_server)
```

## Desarrollo y Contribución

### Compilar con información adicional
```bash
# Verificar sintaxis
make check-syntax

# Información del sistema
make info

# Crear backup antes de modificar
make backup
```

### Testing
```bash
# Tests básicos
make test

# Test de carga (múltiples clientes)
for i in {1..5}; do ./bin/microdb_client -h localhost &; done
```

## Licencia

Proyecto desarrollado para fines educativos. Sistema de Micro Base de Datos con Cliente-Servidor usando sockets TCP/IP.

---

**Autor**: Sistema desarrollado como ejercicio de programación en C con sockets y concurrencia.
**Fecha**: 2025
**Versión**: 1.0
