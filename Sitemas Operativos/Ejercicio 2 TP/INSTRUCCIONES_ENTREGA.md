# Instrucciones de Entrega y Uso

## Para el Evaluador

### 1. Contenido de la Entrega

Este directorio contiene la implementación completa del **Ejercicio 2: Cliente-Servidor de Micro Base de Datos con Transacciones**.

**Archivos incluidos:**

```
✓ server.c              - Código fuente del servidor (21 KB)
✓ client.c              - Código fuente del cliente (5.7 KB)
✓ Makefile              - Sistema de compilación
✓ database.csv          - Base de datos de ejemplo
✓ README.md             - Manual de usuario completo
✓ TESTING.md            - Guía de pruebas y monitoreo
✓ ARCHITECTURE.md       - Documentación técnica
✓ RESUMEN.md            - Resumen ejecutivo del proyecto
✓ test_server.sh        - Script de prueba del servidor
✓ test_concurrent.sh    - Script de pruebas de concurrencia
✓ monitor.sh            - Script de monitoreo en tiempo real
✓ server.conf           - Archivo de configuración opcional
✓ .gitignore           - Control de versiones
```

### 2. Requisitos del Sistema

- **Sistema Operativo**: Linux (probado en distribuciones recientes)
- **Compilador**: gcc con soporte C99 o superior
- **Bibliotecas**: pthread (POSIX threads)
- **Herramientas opcionales**: netstat, lsof, htop (para monitoreo)

### 3. Compilación (30 segundos)

```bash
# Navegar al directorio del proyecto
cd "Ejercicio 2 TP"

# Compilar todo
make

# Verificar compilación exitosa
ls -lh server client
```

**Resultado esperado:**

```
server: ELF 64-bit LSB pie executable
client: ELF 64-bit LSB pie executable
```

### 4. Prueba Rápida (2 minutos)

#### Opción A: Prueba Automática

```bash
# Terminal 1: Iniciar servidor con monitoreo
./test_server.sh

# Terminal 2: Ejecutar pruebas de concurrencia
./test_concurrent.sh
```

#### Opción B: Prueba Manual

**Terminal 1 - Servidor:**

```bash
./server -p 8080 -n 3 -m 5
```

**Terminal 2 - Cliente 1:**

```bash
./client -s 127.0.0.1 -p 8080

# Probar comandos:
db> SELECT ALL
db> SELECT City Madrid
db> INSERT 500,TestUser,30,TestCity,IT,50000,5
db> UPDATE 500 Salary 55000
db> DELETE 500
db> QUIT
```

**Terminal 3 - Cliente 2 (para probar concurrencia):**

```bash
./client -s 127.0.0.1 -p 8080
db> SELECT ALL
```

### 5. Prueba de Transacciones (3 minutos)

**Terminal 1 - Cliente con transacción:**

```bash
./client -s 127.0.0.1 -p 8080

db> BEGIN TRANSACTION
OK
Transacción iniciada

db> UPDATE 1 Salary 80000
OK
Registro actualizado correctamente

# NO ejecutar COMMIT todavía
```

**Terminal 2 - Cliente bloqueado:**

```bash
./client -s 127.0.0.1 -p 8080

db> SELECT ALL
ERROR: Existe una transacción activa. Reintente más tarde.
```

**Terminal 1 - Confirmar transacción:**

```bash
db> COMMIT TRANSACTION
OK
Transacción confirmada
```

**Terminal 2 - Ahora funciona:**

```bash
db> SELECT ALL
OK
[datos mostrados correctamente con los cambios aplicados]
```

### 6. Verificación de Monitoreo (2 minutos)

```bash
# Ver proceso del servidor
ps aux | grep "./server"

# Ver threads (debería haber 1 principal + 1 por cliente conectado)
ps -eLf | grep "./server" | grep -v grep

# Ver socket en escucha
netstat -tlnp | grep 8080
# O alternativamente:
lsof -i :8080

# Ver conexiones establecidas
netstat -tan | grep 8080 | grep ESTABLISHED

# Ver archivos abiertos
lsof -p $(pgrep -f "./server")
```

### 7. Verificación de Limpieza de Recursos

```bash
# Antes de cerrar el servidor, capturar estado
ps aux | grep server > antes.txt
lsof | grep database >> antes.txt
netstat -tan | grep 8080 >> antes.txt

# Cerrar servidor con Ctrl+C
# Presionar Ctrl+C en la terminal del servidor

# Verificar limpieza
ps aux | grep server  # No debería mostrar procesos
lsof | grep database  # No debería mostrar archivos abiertos
netstat -tan | grep 8080  # No debería mostrar sockets
ls *.tmp  # No debería haber archivos temporales
```

### 8. Puntos de Evaluación Cumplidos

| Criterio                 | Evidencia                              | Ubicación                   |
| ------------------------ | -------------------------------------- | --------------------------- |
| **Parámetros validados** | Ejecutar `./server -h` o `./client -h` | server.c líneas 96-134      |
| **Conexión TCP/IP**      | Ver con `netstat` o `lsof`             | Sección 6 de este documento |
| **Concurrencia**         | Múltiples clientes simultáneos         | test_concurrent.sh          |
| **Transacciones**        | BEGIN/COMMIT con locks                 | Sección 5 de este documento |
| **Manejo de errores**    | Desconexiones controladas              | TESTING.md secciones 4 y 5  |
| **Limpieza de recursos** | Verificación post-cierre               | Sección 7 de este documento |
| **Makefile**             | Compilación exitosa                    | `make clean && make`        |
| **Documentación**        | README, TESTING, ARCHITECTURE          | Raíz del proyecto           |

### 9. Características Destacables

1. **Sistema de Transacciones Completo**

   - Lock exclusivo durante transacción
   - Liberación automática en caso de desconexión
   - Mensajes de error claros para clientes bloqueados

2. **Manejo Robusto de Errores**

   - Validación completa de parámetros
   - Manejo de señales (SIGINT, SIGTERM, SIGPIPE)
   - Desconexiones inesperadas no afectan al servidor

3. **Documentación Exhaustiva**

   - README.md: Manual de usuario
   - TESTING.md: Guía de pruebas paso a paso
   - ARCHITECTURE.md: Documentación técnica detallada
   - RESUMEN.md: Resumen ejecutivo

4. **Scripts de Prueba Automatizados**

   - test_server.sh: Verificación del servidor
   - test_concurrent.sh: Pruebas de concurrencia
   - monitor.sh: Monitoreo en tiempo real

5. **Código Limpio y Bien Estructurado**
   - Comentarios descriptivos
   - Funciones modulares
   - Manejo de memoria sin leaks
   - Sin warnings críticos de compilación

### 10. Casos de Prueba Sugeridos

#### Caso 1: Operaciones Básicas (2 min)

```bash
./client -s 127.0.0.1 -p 8080
db> SELECT ALL
db> SELECT Name Carlos
db> QUIT
```

**Esperado**: Consultas exitosas con resultados

#### Caso 2: CRUD Completo (3 min)

```bash
db> INSERT 999,Test,25,Test,Test,50000,5
db> SELECT ID 999
db> UPDATE 999 Name Updated
db> SELECT ID 999
db> DELETE 999
db> SELECT ID 999
```

**Esperado**: Todas las operaciones exitosas

#### Caso 3: Transacción con Bloqueo (5 min)

- Ver Sección 5 de este documento
  **Esperado**: Cliente 2 bloqueado hasta que Cliente 1 haga COMMIT

#### Caso 4: Servidor Lleno (2 min)

```bash
# Iniciar servidor con max_clients=2
./server -p 8080 -n 2 -m 5

# Abrir 3 clientes simultáneamente
# Terminal 1: ./client -s 127.0.0.1 -p 8080
# Terminal 2: ./client -s 127.0.0.1 -p 8080
# Terminal 3: ./client -s 127.0.0.1 -p 8080
```

**Esperado**: Tercer cliente recibe "Servidor lleno"

#### Caso 5: Desconexión Abrupta (3 min)

```bash
# Cliente 1: Iniciar transacción
db> BEGIN TRANSACTION
# Presionar Ctrl+C (desconexión abrupta)

# Cliente 2: Verificar que puede operar
db> SELECT ALL
```

**Esperado**: Lock liberado automáticamente, Cliente 2 puede operar

### 11. Comandos de Limpieza

```bash
# Si algo quedó corriendo
pkill -9 server
pkill -9 client

# Limpiar compilación
make clean

# Limpiar logs (si existen)
rm -f *.log

# Restaurar database.csv original (si fue modificado)
git checkout database.csv  # Si está en git
```

### 12. Ayuda Rápida

**Ver ayuda del servidor:**

```bash
./server -h
```

**Ver ayuda del cliente:**

```bash
./client -h
```

**Ver comandos disponibles en el cliente:**

```bash
# Después de conectar:
db> HELP
```

### 13. Estructura de Evaluación Sugerida

1. **Compilación (5%)**: `make` exitoso
2. **Ejecución básica (10%)**: Servidor inicia, cliente conecta
3. **Operaciones CRUD (20%)**: SELECT, INSERT, UPDATE, DELETE
4. **Concurrencia (20%)**: Múltiples clientes simultáneos
5. **Transacciones (25%)**: BEGIN/COMMIT con locks
6. **Manejo de errores (10%)**: Desconexiones, errores, validaciones
7. **Limpieza de recursos (10%)**: Sin leaks, recursos cerrados
8. **Documentación (10%)**: README, comentarios, guías

### 14. Contacto para Consultas

**Estudiante**: Agustin  
**Universidad**: UNLAM  
**Materia**: Sistemas Operativos  
**Entrega**: Octubre 2025

---

## Para el Estudiante

### Antes de Entregar

- [ ] Compilar limpiamente: `make clean && make`
- [ ] Ejecutar pruebas automatizadas: `./test_server.sh` y `./test_concurrent.sh`
- [ ] Verificar que todos los archivos están presentes
- [ ] Revisar que database.csv existe y tiene datos
- [ ] Probar en una máquina limpia si es posible
- [ ] Verificar que no hay rutas absolutas hardcodeadas
- [ ] Confirmar que .gitignore excluye binarios

### Checklist de Entrega

**Archivos Obligatorios:**

- [x] server.c
- [x] client.c
- [x] Makefile
- [x] database.csv
- [x] README.md

**Archivos Adicionales (recomendados):**

- [x] TESTING.md
- [x] ARCHITECTURE.md
- [x] Scripts de prueba
- [x] .gitignore

**Verificaciones Finales:**

- [x] Compila sin errores
- [x] Ejecuta correctamente
- [x] Soporta múltiples clientes
- [x] Transacciones funcionan
- [x] Limpia recursos al cerrar
- [x] Documentación completa

---

## Resumen de 30 Segundos

```bash
# Compilar
make

# Terminal 1: Servidor
./server -p 8080

# Terminal 2: Cliente
./client -s 127.0.0.1 -p 8080
db> SELECT ALL
db> QUIT

# Limpiar
make clean
```

✅ **Todo listo para entrega y evaluación**
