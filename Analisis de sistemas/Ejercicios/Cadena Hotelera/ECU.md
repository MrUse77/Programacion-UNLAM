# Registrar Reserva

# Tipo de caso de uso: base

# Descripcion del CU: El cliente indica hotel 

# Actor Princial: cliente

# Actor Secundario: No aplica

# Autor: Agustin 

# Fecha de creacion: 11/10/2025

# Precondiciones: Iniciar Sesion

# Punto de extension: Registrar Pago

# Flujo Normal

	1. El cliente inicia el CU

	2. El sistema muestra la interfaz con fecha, hora, datos del cliente y hoteles

	3. El cliente selecciona el hotel

	4. El sistema muestra las habitaciones disponibles para ese hotel

	5. El cliente selecciona el periodo (fecha desde y fecha hasta) 

	6. El sistema muestra los tipos de habitaciones disponibles en ese periodo

	7. Para cada tipo de habitacion
		
		7.1 El cliente selecciona un tipo de habitacion
		
		7.2 El sistema habilita el boton de "Agregar habitacion"
	
		7.3 El cliente presiona el boton "Agregar habitacion"

		7.4 El sistema agrega la habitacion a la reserva y muestra las habitaciones seleccionadas

	8. El cliente confirma la reserva
	
	9. El sistema muestra los datos de la reserva y finaliza el CU

	[Se extiende en Registrar Pago]

# Flujo Alternativo

	* En cualquier momento antes del paso 8, el cliente puede cancelar la operacion y finalizar el CU

	A0. [El cliente ingresa una fecha invalida (fecha desde mayor a fecha hasta)]

		5.1. El sistema muestra un mensaje de error indicando que la fecha es invalida

		5.2. El cliente ingresa nuevamente las fechas

		5.3. Continua en el paso 6 del flujo normal

	A1. [No hay habitaciones disponibles en el periodo seleccionado]
		
		6.1. El sistema muestra un mensaje indicando que no hay habitaciones disponibles

		6.2. Continua en el paso 5 del flujo normal
	A2. [El cliente selecciona una habitacion reservada]

		7.1. El sistema muestra un mensaje indicando que la habitacion ya fue reservada

		7.2. Continua en el paso 6 del flujo normal

	A3. [No selecciono una habitacion]

		8.1. El sistema muestra un mensaje indicando que debe seleccionar al menos una habitacion

		8.2. Continua en el paso 7 del flujo normal

# Postcondiciones: Reserva registrada, se actualizan las habitaciones disponibles en el hotel y período seleccionado.

# Realizar Pago

# Tipo de caso de uso: Extend

# Descripcion del CU: El cliente ingresa los datos de la tarjeta de credito y realiza el pago de la reserva

# Actor Princial: cliente

# Actor Secundario: Sitema de Tarjeta de Credito

# Autor: Agustin

# Fecha de creacion: 11/10/2025

# Precondiciones: Reservar Habitacion

# Punto de extension: No aplica
 
# Flujo Normal

	1. El cliente inicia el CU

	2. El sistema muestra la interfaz con los datos de la reserva y el monto a pagar

	3. El cliente ingresa los datos de la tarjeta de credito (numero, titular, fecha de vencimiento, codigo de seguridad)

	4. El cliente presiona el boton "Pagar"
	[Se incluye comunicacion con el sistema de tarjeta de credito]

	5. El sistema procesa el pago, confirma la reserva y muestra un mensaje de exito

	6. El sistema finaliza el CU

# Flujo Alternativo

	* En cualquier momento antes del paso 4, el cliente puede cancelar la operacion y finalizar el CU

	B0. [El cliente ingresa datos de tarjeta invalidos]

		3.1. El sistema muestra un mensaje de error indicando que los datos son invalidos

		3.2. Continua en el paso 3 del flujo normal

	B1. [El pago es rechazado por el sistema de tarjeta de credito]

		5.1. El sistema muestra un mensaje indicando que el pago fue rechazado

		5.2. Continua en el paso 3 del flujo normal

# Postcondiciones: Pago registrado, se confirma la reserva


