# CU: Cargar Combustible

## Tipo de CU

Base

## Objetivo del CU

El conductor pueda cargar combustible en su vehiculo, y dependiendo de la cantidad de cargas realizadas en el mes, obtener descuentos en la proxima carga.

## Actor Principal

Conductor

## Actor secundario

--

## Autor

Agustin

## Fecha de creacion

12 de octubre del 2025

## Precondicion

Registrar Nafta, Registrar Boca Expendio, Registrar Descuentos

## Puntos de extension

- Aplicar Descuento

- Registrar Vehiculo

## Flujo Normal

1. El conductor inicia el CU ingresando los datos de la tarjeta de credito.
   [Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]

2. El sistema habilita los combustibles disponibles en la boca de expendio.

3. El conductor ingresa el numero de patente, selecciona el tipo de combustible y el monto a cargar.

4. El sistema muestra el monto a pagar y habilita el expendio.

5. El conductor carga el combustible en su vehiculo y coloca la manguera en su lugar.
   [Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]

6. El sistema registra la carga, el, actualiza el historial de cargas del vehiculo, debita el importe, imprime el comprabante y finaliza el caso de uso.

## Post Condicion

Conductor carga combustible en su vehiculo, se registra la carga en el sistema para ese vehiculo.

## Flujo alternativo

El conductor puede cancelar el CU en cualquier momento antes del paso 8.

### A0. (Descuento Aplicado)

    4.1 [Se extiende del CU: Aplicar Descuento]

    4.2  Continua en el punto 6 del flujo normal

### A1. (Datos de tarjeta invalidos)

    4.1. El sistema muestra un mensaje: "Datos de tarjeta invalidos, ingrese nuevamente"

    4.2. Continua en el punto 5 del flujo normal

### A2. (Saldo insuficiente)

    4.1. El sistema muestra un mensaje: "Saldo insuficiente, ingrese otra tarjeta o cancele la operacion"

    4.2. Continua en el punto 5 del flujo normal

### A3. (Tipo de combustible no disponible)

    6.1. El sistema muestra un mensaje: "Tipo de combustible no disponible, seleccione otro tipo"

    6.2 Continua en el punto 7 del flujo normal o finaliza el CU
