# CU: Comprar entrada

## Tipo de CU

Base

## Objetivo del CU

El cliente puede comprar una entrada para el evento a traves de la pagina web, pagando con su tarjeta de credito registrada y teniendo la posibilidad de cargar saldo en su cuenta.

## Actor Principal

Cliente

## Actor secundario

--

## Autor

Agustin

## Fecha de creacion

12 de octubre del 2025

## Precondicion

Iniciar Sesion

## Puntos de extension

Cargar Saldo

## Flujo Normal

    1. El cliente ingresa a la pagina web e inicia el caso de uso.

    2. El sistema muestra los datos del cliente y carga los tipos de entrada disponibles.

    3. El cliente selecciona el tipo de entrada.

    4. El sistema muestra el monto a pagar.

    5. El cliente ingresa los datos de la tarjeta de credito y confirma la compra.
    [Se incluye comunicacion con el sistema de la compañia de tarjeta de credito]

    6. El sistema registra la compra, debita el importe y finaliza el caso de uso.

## Flujo alternativo

    El cliente puede finaliza el caso de uso en cualquier momento antes del paso 5.

### A0. (Saldo insuficiente en la tarjeta)

    	5.1. El sistema muestra un mensaje: "Saldo insuficiente"

    	5.2. Fin del CU

### A1. (No hay entradas disponibles)

    	3.1. El sistema muestra un mensaje: "No hay entradas disponibles"

    	3.2. Fin del CU

### A2.(Tarjeta de credito no valida)

    	5.1. El sistema muestra un mensaje: "Tarjeta de credito no valida"

    	5.2. Fin del CU

### A3. (Cargar saldo)

    	4.1 Se extiende el CU: Cargar Saldo

    	4.2 Continua en el punto 5 del flujo normal
