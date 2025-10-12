# CU: Cargar combustible

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

Registrar Nafta, Registrar Boca Expendio

## Puntos de extension

- Aplicar Descuento

- Registrar Vehiculo

## Flujo Normal

	1. El conductor llega a la estacion de servicio y se dirige a la boca de expendio para iniciar el caso de uso.

	2. El sistema muestra la interfaz la fecha y hora, ademas del numero de boca de expendio.

	3. El conductor ingresa el numero de patente.

	4. El sistema muestra el modelo del vehiculo y el nombre del conductor.
	
	5. El conductor ingresa los datos de la tarjeta de credito y el monto a cargar.
	[Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]
	
	6. El sistema carga los combustibles disponibles en la boca de expendio, muestra el monto a pagar y los puntos a obtener.
	
	7. El conductor selecciona el tipo de combustible.

	8. El sistema habilita el expendio de combustible.
	
	9. El conductor carga el combustible en su vehiculo y coloca la manguera en su lugar.

	10. El sistema registra la carga, actualiza el historial de cargas del vehiculo, debita el importe y finaliza el caso de uso.

## Post Condicion

Conductor carga combustible en su vehiculo, se registra la carga en el sistema para ese vehiculo y obtiene puntos en la tarjeta

## Flujo alternativo

El conductor puede cancelar el CU en cualquier momento antes del paso 8.


### A0. (Numero de patente inexistente)

	1.1  [Se extiende del CU: Registrar Vehiculo]

	1.2  Continua en el punto 4 del flujo normal

### A1. (Datos de tarjeta invalidos)
	
	5.1. El sistema muestra un mensaje: "Datos de tarjeta invalidos, ingrese nuevamente"

	5.2. Continua en el punto 5 del flujo normal

### A2. (Saldo insuficiente)

	5.1. El sistema muestra un mensaje: "Saldo insuficiente, ingrese otra tarjeta o cancele la operacion"

	5.2. Continua en el punto 5 del flujo normal

### A3. (Tipo de combustible no disponible)

	8.1. El sistema muestra un mensaje: "Tipo de combustible no disponible, seleccione otro tipo"
	
	8.2 Continua en el punto 7 del flujo normal o finaliza el CU

