# Registrar Despacho de Bulto

# Tipo de caso de uso: base

# Descripcion del CU: El despachante registra un bulto para un vuelo

# Actor Princial: Despachante

# Actor Secundario: No aplica

# Autor: Agustin Doricich

# Fecha de creacion: 11/10/2025

# Precondiciones: Iniciar Sesion

# Punto de extension: No aplica

# Flujo Normal

	1. El despachante inicia el CU

	2. El sistema muestra la interfaz con fecha, hora y carga los vuelos de las proximas 2 horas

	3. El despachante selecciona el vuelo al que desea registrar el bulto

	4. El sistema muestra los datos del vuelo seleccionado

	5. El despachante ingresa el numero de boleto del pasajero

	6. El sistema muestra los datos del pasajeros y habilita el ingreso de bultos

	7. Por cada bulto:

		7.1. El despachante selecciona el tipo de bulto

		7.2. El sistema muestra los datos y descripcion de la categoria seleccionada

		7.3. El despachante ingresa el peso del bulto

		7.4. El sistema imprimer la etiqueta con un codigo QR para el bulto
	
	8. El despachante presion "Finalizar despacho"
	9. El sistema muestra un resumen del despacho, confirma la operacion, imprime el recibo y finaliza el CU

# Postcondicion: El bulto queda registrado en el sistema y se actualiza el estado del boleto del pasajero

# Flujo Alternativo

	*En cualquier momento antes del paso  7, el actor puede cancelar el CU*
	
	A1.[No existen vuelos]

		3.1. Se desprende del flujo normal en el paso 3

		3.2. El sistema muestra un mensaje indicando que no hay vuelos proximos y finaliza el CU
	
	A2.[Numero de boleto invalido]
		
		5.1. Se desprende del flujo normal en el paso 5
		
		5.2 El sistema muestra un mensaje indicando que el numero de boleto es invalido y vuelve al paso 5 del flujo normal

