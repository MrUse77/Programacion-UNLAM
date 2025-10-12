
# CU: Alquilar scooter

## Tipo de CU

Base

## Objetivo del CU

El cliente puede alquilar un scooter electrico, escaneado un codigo QR en el scooter y pagando con su tarjeta de credito registrada en la aplicacion movil.

## Actor Principal

Cliente

## Actor secundario

--

## Autor

Agustin

## Fecha de creacion

12 de octubre del 2025

## Precondicion

Iniciar Sesion, Registrar Scooter

## Puntos de extension

No aplica

## Flujo Normal

	1. El cliente se acerca a un scooter, escanea el codigo QR e inicia el caso de uso.
	[Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]

	2. El sistema desbloquea el scooter y registra el viaje.

	3. El conductor finaliza el viaje escaneando devuelta el codigo QR.

	4. El sistema muestra el saldo a debitarse, se realiza el pago y finaliza el caso de uso.
	[Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]

## Post Condicion

El cliente ha alquilado un scooter y se le ha debitado el importe correspondiente.

## Flujo alternativo

### A0. (Saldo insuficiente en la tarjeta)

	2.1. El sistema muestra un mensaje: "Saldo insuficiente"
	
	2.2. Fin del CU

### A1. (Scooter fuera de uso por 10 minutos)

	3.1 El sistema finaliza automaticamente el viaje, realiza el pago y finaliza el caso de uso.
	[Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]

### A2. (Scooter fuera de servicio)

	2.1. El sistema muestra un mensaje: "Scooter fuera de servicio"

	2.2. Fin del CU

### A3. (Fuera de horario)

	2.1. El sistema muestra un mensaje: "Fuera de horario de uso"

	2.2. Fin del CU

