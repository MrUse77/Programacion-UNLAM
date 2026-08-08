# Reservar Cancha

# Tipo de caso de uso: base

# Descripcion del caso de uso: El usuario indica la zona, tipo de cancha, fecha y hora para realizar la reserva, selecciona la cancha correspondiente y luego puede mandar una invitacion a sus amigos antes de concretar la reserva.

# Actor Principal: Usuario

# Actor Secundario: No aplica

# Autor: Agustin

# Fecha de creacion: 11/10/2025

# Precondicion: Iniciar Sesion, Registrar cancha, Registrar Tarifa

# Punto de extension: Invitar amigos

# Flujo normal

    1. El usuario inicia el CU

    2. El sistema carga las opciones de zonas y tipos de canchas disponibles

    3. El usuario selecciona la zona y tipo de cancha

    4. El sistema muestra las canchas disponibles con sus horarios

    5. El usuario selecciona la cancha, fecha y hora deseada

    6. El sistema muestra un resumen de la reserva y el costo total

    7. El usuario confirma la reserva y realiza el pago

    8. El sistema envia un mensaje por whatsapp al usuario e invitados con el mensaje: "SALE PARTIDAZO" y finaliza el CU

# Flujo alternativo

    * En cualquier momento antes del paso 7, el usuario puede optar por cancelar la reserva y finalizar el CU

    A0. [El usuario no encuentra la zona o tipo de cancha deseada]

    	4.1. El sistema muestra un mensaje indicando que no hay opciones disponibles

    	4.2. El usuario puede optar por volver al paso 3 o finalizar el CU

    A1. [El usuario selecciona una cancha, fecha u hora no disponible]

    	6.1. El sistema muestra un mensaje indicando la indisponibilidad

    	6.2. El usuario puede optar por volver al paso 5 o finalizar el CU

    A2. [El usuario invita amigos]

    	8.1. [Se extiende del CU: invitar a amigos]

    	8.2. El sistema vuelve al paso 8 del CU "Reservar Cancha"

# Postcondicion: Reserva realizada
