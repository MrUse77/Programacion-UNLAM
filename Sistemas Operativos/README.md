# Trabajo Práctico - Sistemas Operativos

```
╔══════════════════════════════════════════════════════════════════════════╗
║                                                                          ║
║                   UNIVERSIDAD NACIONAL DE LA MATANZA                     ║
║                              UNLAM - UNLaM                               ║
║                                                                          ║
║                        DEPARTAMENTO DE INGENIERÍA                        ║
║                     E INVESTIGACIONES TECNOLÓGICAS                       ║
║                                                                          ║
╠══════════════════════════════════════════════════════════════════════════╣
║                                                                          ║
║                          SISTEMAS OPERATIVOS                             ║
║                                                                          ║
║                         TRABAJO PRÁCTICO INTEGRADOR                      ║
║                                                                          ║
╠══════════════════════════════════════════════════════════════════════════╣
║                                                                          ║
║  EJERCICIO 1: Sistema de Generación Distribuida de Datos CSV con IPC     ║
║  EJERCICIO 2: Sistema Cliente-Servidor de Micro Base de Datos            ║
║                                                                          ║
╠══════════════════════════════════════════════════════════════════════════╣
║                                                                          ║
║  INTEGRANTES:                                                            ║
║    • ESPADA RODRIGUEZ, FEDERICO                                          ║
║    • DORICICH, AGUSTIN FRANCISCO                                         ║
║    • NIEVA, ALEJO NICOLAS                                                ║
║    • GATTI, GONZALO ALEJO                                                ║
║    • MACIEL, KEVIN DANIEL                                                ║
║    • DE ROGATIS, RAMIRO JAVIER                                           ║
║                                                                          ║
║  COMISIÓN: 02-3009                                                       ║
║                                                                          ║
║                                                                          ║
║  CUATRIMESTRE: 2° Cuatrimestre 2025                                      ║
║  FECHA DE ENTREGA: [Fecha]                                               ║
║                                                                          ║
╚══════════════════════════════════════════════════════════════════════════╝
```

---

## 📋 Índice General

- [Ejercicio 1: Generación Distribuida con IPC](#ejercicio-1-sistema-de-generación-distribuida-de-datos-csv-con-ipc)
- [Ejercicio 2: Cliente-Servidor con Transacciones](#ejercicio-2-sistema-cliente-servidor-de-micro-base-de-datos-con-transacciones)
- [Documentación Completa](#documentación-completa)
- [Conceptos Implementados](#conceptos-implementados)

---

## 📊 Información de los Proyectos

### Ejercicio 1: Sistema de Generación Distribuida de Datos CSV con IPC

**Descripción**: Sistema multi-proceso que genera datos aleatorios en formato CSV utilizando comunicación entre procesos (IPC) mediante memoria compartida System V y semáforos.

**Tecnologías**:

- Procesos concurrentes (`fork()`)
- Memoria compartida System V (`shmget`, `shmat`)
- Semáforos System V (`semget`, `semop`)
- Sincronización con mutex
- Manejo de señales (SIGINT, SIGTERM, SIGCHLD)

**Conceptos de SO**:

- IPC (Inter-Process Communication)
- Sincronización y secciones críticas
- Productor-Consumidor
- Gestión de recursos compartidos
- Control de concurrencia

### Ejercicio 2: Sistema Cliente-Servidor de Micro Base de Datos con Transacciones

**Descripción**: Sistema distribuido de base de datos que utiliza sockets TCP/IP para permitir consultas y modificaciones remotas sobre un archivo CSV, con soporte para transacciones ACID y control de concurrencia.

**Tecnologías**:

- Sockets TCP/IP (cliente-servidor)
- Threads POSIX (`pthread`)
- Sincronización con mutex
- Sistema de transacciones con locks
- Protocolo de comandos SQL-like

**Conceptos de SO**:

- Arquitectura Cliente-Servidor
- Programación con sockets
- Concurrencia con threads
- Transacciones y control de acceso
- Manejo de múltiples clientes simultáneos

---

## 🚀 Ejercicio 1: Generación Distribuida con IPC

### Características Principales

- ✅ **Múltiples procesos generadores** trabajando concurrentemente
- ✅ **Memoria compartida** para comunicación entre procesos
- ✅ **3 semáforos** para sincronización (IDs, Buffer, Write)
- ✅ **Generación de datos aleatorios** (nombres, ciudades, salarios, etc.)
- ✅ **Control de duplicados** garantizado por sincronización
- ✅ **Terminación controlada** con manejo de señales
- ✅ **Limpieza automática** de recursos IPC

### Estructura del Proyecto

```
Ejercicio 1 TP/
├── src/
│   ├── main.c              # Proceso principal e IPC
│   ├── coordinator.c       # Proceso coordinador
│   ├── generators.c        # Procesos generadores
│   └── includes/
│       ├── variables.h     # Estructuras y definiciones
│       ├── coordinator.h   # Interface coordinador
│       └── generator.h     # Interface generadores
├── bin/
│   └── DataGenerator       # Ejecutable
├── Makefile                # Sistema de compilación
├── README.md               # Manual completo
├── ARCHITECTURE.md         # Documentación técnica
├── QUICKSTART.md           # Guía rápida
└── RESUMEN.md              # Resumen ejecutivo

```

### Compilación y Ejecución

```bash
cd "Ejercicio 1 TP"

# Compilar
make

# Ejecutar con 4 generadores y 1000 registros
./bin/DataGenerator 4 1000

# O usar el Makefile
make init

# Ver ayuda
make help
```

### Documentación Específica

- **[README.md](Ejercicio%201%20TP/README.md)**: Manual completo de usuario
- **[ARCHITECTURE.md](Ejercicio%201%20TP/ARCHITECTURE.md)**: Documentación técnica detallada
- **[QUICKSTART.md](Ejercicio%201%20TP/QUICKSTART.md)**: Guía de inicio rápido (60 segundos)
- **[RESUMEN.md](Ejercicio%201%20TP/RESUMEN.md)**: Resumen ejecutivo del proyecto
- **[TESTING.md](Ejercicio%201%20TP/TESTING.md)**: Guía completa de testing

---

## 🌐 Ejercicio 2: Cliente-Servidor con Transacciones

### Características Principales

- ✅ **Arquitectura Cliente-Servidor** con sockets TCP/IP
- ✅ **Múltiples clientes concurrentes** usando threads POSIX
- ✅ **Sistema de transacciones** (BEGIN/COMMIT) con locks exclusivos
- ✅ **Operaciones CRUD**: SELECT, INSERT, UPDATE, DELETE
- ✅ **Control de concurrencia** con mutex y locks
- ✅ **Cola de espera** para clientes cuando servidor está lleno
- ✅ **Manejo robusto de errores** y desconexiones

### Estructura del Proyecto

```
Ejercicio 2 TP/
├── src/
│   ├── server.c            # Servidor multi-threaded
│   └── client.c            # Cliente interactivo
├── bin/
│   ├── server              # Ejecutable servidor
│   └── client              # Ejecutable cliente
├── Makefile                # Sistema de compilación
├── server.conf             # Configuración
├── README.md               # Manual completo
├── ARCHITECTURE.md         # Documentación técnica
├── QUICKSTART.md           # Guía rápida
└── RESUMEN.md              # Resumen ejecutivo
```

### Compilación y Ejecución

```bash
cd "Ejercicio 2 TP"

# Compilar servidor y cliente
make

# Terminal 1: Iniciar servidor
./bin/server -p 8080 -n 5 -m 10

# O usar Makefile con parámetros
make run-server PORT=8080 MAX_CLIENTS=5 MAX_QUEUE=10

# Terminal 2: Conectar cliente
./bin/client -s 127.0.0.1 -p 8080

# O usar Makefile
make run-client

# Ver ayuda
make help
```

### Documentación Específica

- **[README.md](Ejercicio%202%20TP/README.md)**: Manual completo con comandos SQL
- **[ARCHITECTURE.md](Ejercicio%202%20TP/ARCHITECTURE.md)**: Arquitectura y protocolo técnico
- **[QUICKSTART.md](Ejercicio%202%20TP/QUICKSTART.md)**: Guía de inicio rápido (60 segundos)
- **[RESUMEN.md](Ejercicio%202%20TP/RESUMEN.md)**: Resumen ejecutivo con métricas

---

## 🛠️ Compilación y Ejecución

### Requisitos del Sistema

- **Sistema Operativo**: Linux (Ubuntu, Debian, Fedora, etc.)
- **Compilador**: GCC 4.8 o superior
- **Make**: GNU Make
- **Bibliotecas**:
  - libc (Standard C Library)
  - pthread (POSIX Threads)
  - System V IPC (para Ejercicio 1)

### Verificar Requisitos

```bash
# Verificar GCC
gcc --version

# Verificar make
make --version

# Verificar soporte IPC (Ejercicio 1)
ipcs -l

# Verificar soporte de threads (Ejercicio 2)
ldconfig -p | grep pthread
```

### Compilación Rápida

```bash
# Ejercicio 1
cd "Ejercicio 1 TP"
make
./bin/DataGenerator 4 1000

# Ejercicio 2
cd "Ejercicio 2 TP"
make
# Terminal 1
make run-server
# Terminal 2
make run-client
```

---

## 📚 Documentación Completa

Cada ejercicio cuenta con documentación exhaustiva:

### Documentación del Ejercicio 1

| Archivo             | Descripción                    | Tamaño |
| ------------------- | ------------------------------ | ------ |
| **README.md**       | Manual completo de usuario     | ~15 KB |
| **ARCHITECTURE.md** | Arquitectura técnica detallada | ~25 KB |
| **QUICKSTART.md**   | Guía rápida de 60 segundos     | ~5 KB  |
| **RESUMEN.md**      | Resumen ejecutivo              | ~10 KB |

### Documentación del Ejercicio 2

| Archivo             | Descripción                      | Tamaño |
| ------------------- | -------------------------------- | ------ |
| **README.md**       | Manual completo con comandos SQL | ~15 KB |
| **ARCHITECTURE.md** | Protocolo y arquitectura         | ~30 KB |
| **QUICKSTART.md**   | Guía rápida de 60 segundos       | ~5 KB  |
| **RESUMEN.md**      | Resumen ejecutivo con métricas   | ~12 KB |

### Herramientas Utilizadas

- **GCC**: GNU Compiler Collection
- **Make**: GNU Make
- **GDB**: GNU Debugger
- **Valgrind**: Detección de memory leaks
- **strace**: System call tracing
- **ipcs**: IPC resource viewer

## 📝 Notas Finales

Ambos proyectos han sido desarrollados siguiendo las mejores prácticas de programación en C y aplicando los conceptos fundamentales de Sistemas Operativos. Se incluye documentación exhaustiva.

Para instrucciones detalladas de compilación, ejecución y testing, consultar los archivos `README.md` y `QUICKSTART.md` de cada ejercicio.

---

**Fecha de última actualización**: Octubre 2025

**Universidad Nacional de La Matanza - UNLAM**  
**Departamento de Ingeniería e Investigaciones Tecnológicas**  
**Sistemas Operativos - 2° Cuatrimestre 2025**
