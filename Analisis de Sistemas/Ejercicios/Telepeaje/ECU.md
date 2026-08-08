# CU: Pagar peaje

## Tipo de CU

Base

## Objetivo del CU

El conductor puede pagar el peaje al pasar por la cabina de peaje utilizando el TAG asignado.

## Actor Principal

Conductor

## Actor secundario

Sensor
Alarma

## Autor

Agustin

## Fecha de creacion

13 de octubre del 2025

## Precondicion

Dar de alta TAG

## Puntos de extension

No aplica

## Flujo Normal

1. El conductor pasa por la cabina del peaje e inicia el caso de uso.

2. El sistema muestra la fecha y la hora.

3. El sensos escanea el TAG del vehiculo.

4. El sistema debita el saldo, y cede el paso al vehiculo.

5. Fin del CU

## Flujo alternativo

A0.[Saldo insuficiente]

4.1. El sistema muestra un mensaje: "Saldo insuficiente, por favor abone en efectivo"

4.2. Se activa la alarma.

4.3 El sistema no cede el paso al vehiculo.

4.4. Fin del CU
