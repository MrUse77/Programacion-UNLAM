# 🚀 Inicio Rápido - 60 Segundos

## Compilar y Ejecutar

```bash
# 1. Compilar (10 segundos)
make

# 2. Terminal 1: Iniciar servidor
./server -p 8080 -n 5 -m 10

# 3. Terminal 2: Conectar cliente
./client -s 127.0.0.1 -p 8080

# 4. Probar comandos
db> SELECT ALL
db> SELECT City Madrid
db> INSERT 500,Juan,30,Madrid,IT,50000,5
db> UPDATE 500 Salary 55000
db> DELETE 500
db> QUIT

# 5. Detener servidor (Terminal 1)
Ctrl+C
```

## Prueba de Transacciones

```bash
# Terminal 1: Cliente con transacción
./client -s 127.0.0.1 -p 8080
db> BEGIN TRANSACTION
db> UPDATE 1 Salary 80000
# NO hacer COMMIT todavía

# Terminal 2: Otro cliente (será bloqueado)
./client -s 127.0.0.1 -p 8080
db> SELECT ALL
# ERROR: Existe una transacción activa

# Terminal 1: Confirmar transacción
db> COMMIT TRANSACTION

# Terminal 2: Ahora funciona
db> SELECT ALL
# OK - muestra datos
```

## Ayuda

```bash
./server -h        # Ayuda del servidor
./client -h        # Ayuda del cliente
```

En el cliente, escribir `HELP` para ver comandos disponibles.

## Documentación Completa

- **README.md**: Manual completo
- **TESTING.md**: Guía de pruebas
- **ARCHITECTURE.md**: Documentación técnica
- **INSTRUCCIONES_ENTREGA.md**: Para el evaluador
- **RESUMEN.md**: Resumen ejecutivo

## Requisitos

- Linux con gcc
- pthread library
- make

---

✅ **Todo funciona!** Ver README.md para detalles completos.
