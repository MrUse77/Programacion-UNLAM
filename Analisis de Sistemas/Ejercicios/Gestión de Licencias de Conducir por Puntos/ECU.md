# CU: Labrar Acta de Infraccion

## Tipo de CU

Base

## Objetivo del CU

El agente de transito puede labrar un acta de infraccion a un conductor que haya cometido una infraccion de transito, registrando los datos del conductor, la infraccion cometida, actualizando los puntos en la licencia y generando la multa correspondiente.

## Actor Principal

Agente de Transito

## Actor secundario

--

## Autor

Agustin

## Fecha de creacion

12 de octubre del 2025

## Precondicion

Iniciar Sesion

## Puntos de extension

Retener licencia

## Flujo Normal
	
	1. El agente de transito inicia el caso de uso dandole al boton: "Labrar acta".
	
	2. El sistema muestra los datos del agente y un formulario para cargar los datos del conductor y la infraccion cometida.

	3. El agente ingresa los datos del conductor, patente del auto y el lugar donde se cometio la infraccion.

	4. El sistema muestra la lista de infracciones disponibles.

	5. El agente selecciona la infraccion cometida.

	6. El sistema muestra los puntos a descontar y el monto de la multa.

	7. El agente confirma la infraccion dandole al boton: "Confirmar".
	[Se incluye el caso de uso: Actualizar credito]

	8. El sistema imprime el acta de infraccion y finaliza el caso de uso.

## Postcondiciones:

El conductor tiene una infraccion registrada, se le descuentan los puntos correspondientes y se genera la multa.

## Flujo alternativo
	El agente puede finalizar el caso de uso en cualquier momento antes del paso 7.
	
	A0. [Puntaje de la licencia es 0]
	
		8.1. El sistema muestra un mensaje: "Puntaje de la licencia es 0, se retiene la licencia"
		
		8.2. [Se extiende el caso de uso: Retener licencia]
		
		8.3. Fin del CU

	A1. [tipo infraccion no encontrada]

		6.1. El sistema muestra un mensaje: "Infraccion no encontrada"
		
		6.2. Coninua en el paso 5 del flujo normal





