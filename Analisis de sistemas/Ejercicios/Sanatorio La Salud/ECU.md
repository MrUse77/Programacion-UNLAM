# CU: Sacar Turno

## Tipo de CU

Base

## Objetivo del CU

El paciente puede registrar una reserva de un turno para una consulta medica a de manera online o presencial, seleccionando el medico, la fecha y el horario disponible.

## Actor Principal

Paciente

## Actor secundario

--

## Autor

Agustin

## Fecha de creacion

12 de octubre del 2025

## Precondicion

Iniciar Sesion

## Puntos de extension

No aplica

## Flujo Normal

1. El paciente selecciona Sacar Turno e inicia el CU
2. El sistema muestra la interfaz con fecha, hora, datos del paciente y carga las especialidades.
3. El paciente selecciona la especialidad.
4. El sistema carga las sedes disponibles para esas especialidades.
5. El paciente selecciona la sede donde atenderse.
6. El sistema carga los horarios disponibles.
7. El paciente selecciona el horario de atencion.
8. El sistema muestra un resumen del turno con los datos del paciente, medico, fecha y hora.
9. El paciente confirma el turno.
10. El sistema registra el turno y finaliza el CU.

## Postcondicion

El turno queda registrado en el sistema con los datos del paciente, medico, fecha y hora.

## Flujo Alternativo

- En cualquier momento antes del paso 9, el paciente puede optar por cancelar el proceso y finalizar el CU.

A0. [No hay especialidades disponibles]

2.1. El sistema muestra un mensaje indicando que no hay especialidades disponibles.

2.2. Finaliza el CU.

A1. [No hay sedes disponibles para la especialidad seleccionada]

5.1. El sistema muestra un mensaje indicando que no hay sedes disponibles.

5.2. Finaliza el CU.

A2. [No hay horarios disponibles en la sede seleccionada]

7.1. El sistema muestra un mensaje indicando que no hay horarios disponibles.

7.2. Continua en el paso 5 del flujo normal.

A3. [Se encontro historial con un medico]

4.1. El sistema muestra un mensaje indicando que encontro historial con un medico, muestra sus datos, sede y carga los horarios disponibles.

4.2. Continua en el paso 7 del flujo normal.
