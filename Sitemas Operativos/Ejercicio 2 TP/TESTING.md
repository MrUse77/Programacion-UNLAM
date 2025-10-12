# Guía de Pruebas y Monitoreo

## Compilación y Ejecución Rápida

### Compilar el proyecto

```bash
make
```

### Ejecutar servidor

```bash
# Forma simple
./server -p 8080

# Con todos los parámetros
./server -p 8080 -n 10 -m 20
```

### Ejecutar cliente

```bash
./client -s 127.0.0.1 -p 8080
```

## Scripts de Prueba Automatizados

### 1. Iniciar servidor con monitoreo

```bash
./test_server.sh
```

Este script:

- Verifica requisitos
- Inicia el servidor
- Muestra información del proceso
- Muestra el log en tiempo real

### 2. Pruebas de concurrencia

```bash
# En otra terminal, con el servidor corriendo:
./test_concurrent.sh
```

Este script ejecuta automáticamente:

- Consultas concurrentes
- Prueba de transacciones con bloqueo
- Operaciones CRUD completas

### 3. Monitor en tiempo real

```bash
# En otra terminal:
./monitor.sh
```

Muestra en tiempo real:

- Procesos y threads
- Conexiones de red
- Archivos abiertos
- Uso de recursos

## Ejemplos de Sesiones de Prueba

### Prueba 1: Consultas Básicas

**Terminal 1 - Servidor:**

```bash
./server -p 8080 -n 5 -m 10
```

**Terminal 2 - Cliente:**

```bash
./client -s 127.0.0.1 -p 8080

db> SELECT ALL
db> SELECT Name Carlos
db> SELECT City Madrid
db> QUIT
```

### Prueba 2: Inserción y Modificación

**Cliente:**

```bash
db> INSERT 500,Juan,30,Madrid,IT,50000,5
db> SELECT ID 500
db> UPDATE 500 Salary 55000
db> UPDATE 500 City Barcelona
db> SELECT ID 500
db> DELETE 500
```

### Prueba 3: Transacciones

**Terminal 1 - Cliente con transacción:**

```bash
db> BEGIN TRANSACTION
OK
Transacción iniciada

db> UPDATE 1 Salary 80000
OK
Registro actualizado correctamente

db> UPDATE 2 Salary 90000
OK
Registro actualizado correctamente

db> COMMIT TRANSACTION
OK
Transacción confirmada
```

**Terminal 2 - Otro cliente (bloqueado):**

```bash
db> SELECT ALL
ERROR: Existe una transacción activa. Reintente más tarde.

# Después del COMMIT del otro cliente:
db> SELECT ALL
OK
[datos actualizados...]
```

### Prueba 4: Múltiples Clientes Concurrentes

Abrir 4 terminales y en cada una ejecutar:

**Terminal 1:**

```bash
./client -s 127.0.0.1 -p 8080
db> SELECT City Madrid
```

**Terminal 2:**

```bash
./client -s 127.0.0.1 -p 8080
db> SELECT Department IT
```

**Terminal 3:**

```bash
./client -s 127.0.0.1 -p 8080
db> SELECT Name Carmen
```

**Terminal 4:**

```bash
./client -s 127.0.0.1 -p 8080
db> SELECT ALL
```

Todas las consultas se ejecutan concurrentemente sin problemas.

## Comandos de Monitoreo Linux

### Ver procesos del servidor

```bash
# Buscar proceso servidor
ps aux | grep server

# Ver todos los threads
ps -eLf | grep server

# Contar threads
ps -eLf | grep server | wc -l

# Monitoreo interactivo
htop -p $(pgrep server)
```

### Ver conexiones de red

```bash
# Ver socket en escucha
netstat -tlnp | grep 8080
lsof -i :8080

# Ver conexiones establecidas
netstat -tan | grep 8080 | grep ESTABLISHED

# Contar conexiones activas
netstat -tan | grep 8080 | grep ESTABLISHED | wc -l

# Monitoreo continuo
watch -n 1 'netstat -tan | grep 8080'
```

### Ver archivos abiertos

```bash
# Por proceso
lsof -p $(pgrep server)

# Por puerto
lsof -i :8080

# Solo archivos regulares
lsof -p $(pgrep server) | grep REG
```

### Ver recursos del sistema

```bash
# Uso de CPU y memoria
ps -p $(pgrep server) -o pid,ppid,%cpu,%mem,vsz,rss,stat,time,cmd

# Threads del proceso
top -H -p $(pgrep server)

# Información detallada
cat /proc/$(pgrep server)/status
```

### Ver semáforos y recursos IPC

```bash
# Listar todos los recursos IPC
ipcs -a

# Solo semáforos
ipcs -s

# Solo memorias compartidas
ipcs -m

# Solo colas de mensajes
ipcs -q
```

## Pruebas de Estrés

### Generar múltiples clientes

```bash
# Script para generar 10 clientes simultáneos
for i in {1..10}; do
    (echo "SELECT ALL"; echo "QUIT") | ./client -s 127.0.0.1 -p 8080 > client_$i.log 2>&1 &
done

# Esperar a que terminen
wait

# Ver resultados
cat client_*.log
```

### Prueba de desconexión abrupta

```bash
# Terminal 1 - Cliente
./client -s 127.0.0.1 -p 8080
db> BEGIN TRANSACTION
db> UPDATE 1 Salary 70000

# Presionar Ctrl+C (desconexión abrupta)

# Terminal 2 - Verificar que el servidor sigue funcionando
./client -s 127.0.0.1 -p 8080
db> SELECT ALL
# Debería funcionar normalmente
```

### Llenar servidor

```bash
# Si el servidor tiene max_clients=3, intentar 5 clientes:

# Terminal 1, 2, 3 - Clientes que se conectan
./client -s 127.0.0.1 -p 8080
# Quedarse en el prompt sin hacer QUIT

# Terminal 4, 5 - Intentar conectar
./client -s 127.0.0.1 -p 8080
# Deberían recibir: "ERROR: Servidor lleno. Intente más tarde."
```

## Documentación de Evidencias

### Capturar estado inicial

```bash
# Antes de iniciar pruebas
echo "=== Estado inicial ===" > evidencias.txt
ps aux >> evidencias.txt
netstat -tan >> evidencias.txt
lsof | grep database >> evidencias.txt
ipcs -a >> evidencias.txt
```

### Durante las pruebas

```bash
# Capturar screenshots o copiar salidas de:
- netstat -tlnp | grep 8080
- ps -eLf | grep server
- lsof -p $(pgrep server)
- htop (presionar F2 para configurar, F10 para salir)
```

### Capturar estado final

```bash
# Después de cerrar todo
echo "=== Estado final ===" >> evidencias.txt
ps aux >> evidencias.txt
netstat -tan >> evidencias.txt
lsof | grep database >> evidencias.txt
ipcs -a >> evidencias.txt

# Verificar limpieza
diff <(grep "Estado inicial" -A 100 evidencias.txt) <(grep "Estado final" -A 100 evidencias.txt)
```

## Verificación de Limpieza de Recursos

### Después de cerrar el servidor

```bash
# No debería haber procesos servidor
ps aux | grep server

# No debería haber sockets en el puerto
lsof -i :8080
netstat -tan | grep 8080

# No debería haber archivos temporales
ls -la *.tmp

# No debería haber semáforos huérfanos
ipcs -s
```

### Limpieza manual (si es necesario)

```bash
# Matar procesos
pkill -9 server
pkill -9 client

# Eliminar archivos temporales
rm -f *.tmp *.log

# Limpiar recursos IPC (solo si quedaron huérfanos)
ipcs -s | grep $USER | awk '{print $2}' | xargs -n1 ipcrm -s
```

## Resolución de Problemas Comunes

### Puerto en uso

```bash
# Verificar qué proceso usa el puerto
lsof -i :8080

# Matar el proceso
lsof -ti:8080 | xargs kill -9

# O usar otro puerto
./server -p 8081
```

### Demasiados archivos abiertos

```bash
# Ver límite actual
ulimit -n

# Aumentar límite (temporal)
ulimit -n 4096

# Aumentar permanentemente (editar /etc/security/limits.conf)
```

### Memoria insuficiente

```bash
# Ver memoria disponible
free -h

# Ver uso por proceso
ps aux --sort=-%mem | head
```

## Logs y Debugging

### Ejecutar con valgrind (detección de memory leaks)

```bash
valgrind --leak-check=full --show-leak-kinds=all ./server -p 8080
```

### Ejecutar con strace (rastrear syscalls)

```bash
strace -f -o server_trace.txt ./server -p 8080
```

### Ejecutar con gdb (debugging)

```bash
gdb ./server
(gdb) run -p 8080
# Si hay crash:
(gdb) backtrace
```

## Métricas a Documentar

1. **Tiempo de respuesta**: Medir con `time` command
2. **Throughput**: Número de operaciones por segundo
3. **Concurrencia**: Máximo número de clientes simultáneos
4. **Uso de memoria**: Con `ps` o `top`
5. **Uso de CPU**: Con `ps` o `top`
6. **Número de threads**: Con `ps -eLf`
7. **Conexiones simultáneas**: Con `netstat`
8. **Latencia**: Con `ping` o herramientas custom
