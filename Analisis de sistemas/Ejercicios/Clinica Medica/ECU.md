
# CU: Salir de neonatologia

## Tipo de CU

Base

## Objetivo del CU

La enfermera acompaña a la mama a salir de neonatologia con el bebe teniendo que escanear su iris y la huella del bebe para verificar su identidad, registrando el movimiento en la sala.

## Actor Principal

Mama

## Actor secundario

Alarma?

## Autor

Agustin

## Fecha de creacion

13 de octubre del 2025

## Precondicion

Ingresar a neonatologia

## Puntos de extension

No aplica

## Flujo Normal
	
	1. La mama inicia el caso de uso presinando el boton de "Escanear Iris en la pantalla".
	[Se incluye caso de uso escanear iris].
	
	2. El sistema confirma que hay un bebe en la sala registrado y habilita la entrada de la mama.

	3. La mama ingresa y finaliza el CU.	

## Flujo alternativo 
	
	En cualquier momento antes del paso 4, la mama puede finalizar el caso de uso.
	
	A0. [Acceso fallido]

		2.1. El sistema activa la alarma y notifica al personal de seguridad.

		2.2. Fin del CU
	
	A1. [No hay bebe con iris en su acta de nacimiento]

		2.1. El sistema activa la alarma y notifica al personal de seguridad.

		2.2. Fin del CU

## Postcondicion: La mama ingresa a neonatologia a ver al bebe

# CU: Escanear iris

## Tipo de CU

Base

## Objetivo del CU

La mama puede escanear su iris para verificar su identidad

## Actor Principal

Mama

## Actor secundario

No aplica

## Autor

Agustin

## Fecha de creacion

13 de octubre del 2025

## Precondicion

No aplica

## Puntos de extension

No aplica

## Flujo Normal

	1. La mama inicia el caso de uso acercandose al escaner de iris.

	2. El sistema escanea el iris, verifica la identidad y finaliza el CU.

## Flujo alternativo 

	A0. (Identidad no verificada)

		2.1. El sistema muestra un mensaje: "Identidad no verificada"

		2.2. Fin del CU
	
	A1. (Error en el escaneo)

		2.1. El sistema muestra un mensaje: "Error en el escaneo, intente nuevamente"

		2.2. Continua en el paso 1 del flujo normal.

## Postcondicion: La mama escanea su iris y verifica su identidad






	



