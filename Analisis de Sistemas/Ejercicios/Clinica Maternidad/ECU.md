# CU: Salir de neonatologia

## Tipo de CU

Base

## Objetivo del CU

La enfermera acompaña a la mama a salir de neonatologia con el bebe teniendo que escanear su iris y la huella del bebe para verificar su identidad, registrando el movimiento en la sala.

## Actor Principal

Enfermera

## Actor secundario

Mama, Alarma

## Autor

Agustin

## Fecha de creacion

13 de octubre del 2025

## Precondicion

Ingresar a neonatologia

## Puntos de extension

No aplica

## Flujo Normal

1. La Enfermera presiona el boton de "Registrar salida de neonatologia" e inicia el CU.

2. El sistema muestra la interfaz con fecha y hora actual y habilita el escaner de iris.

3. La mama escanea su iris en el lector biometrico.

4. El sistema habilita el escaner de huella dactilar.

5. El bebe es pasado por el lector de huella dactilar.

6. El sistema confirma que la mama y el bebe son los correctos, y muestra mensaje de exito.

7. La enfermera presiona el boton de "Confirmar salida".

8. El sistema registra el movimiento y finaliza el CU.

## Flujo alternativo

En cualquier momento antes del paso 4, la mama puede finalizar el caso de uso.

A0. [Los datos no coinciden con el acta de nacimiento]

2.1. El sistema muestra un mensaje de error indicando que los datos no coinciden con el acta de nacimiento.

2.2. [Se extiende del caso de uso Notificar]

2.3. Fin del CU

## Postcondicion

La mama sale de neonatologia con el bebe en brazos y se registra la salida en el sistema.
