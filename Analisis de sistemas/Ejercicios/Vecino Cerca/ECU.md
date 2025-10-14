# CU: Registrar Venta

## Tipo de CU

Base

## Objetivo del CU

Registrar una venta en caja desde la identificación del cliente hasta la impresión del ticket, aplicando promociones y cobrando mediante tarjeta de crédito con QR

## Actor Principal

Cajero

## Actor secundario

--

## Autor

Agustin

## Fecha de creacion

7 de octubre del 2025

## Precondicion

Iniciar Sesion, Registrar Producto

## Puntos de extension

Generar TA

## Flujo Normal

	1. El cajero ingresa la tarjeta de afinidad e inicia el caso de uso

	2. El sistema muestra los datos del cliente y habilita el lector.

	3. Por cada producto:

	   3.1. El cajero pasa el codigo de barras del producto.

	   3.2 El sistema muestra el nombre del producto, codigo, precio, descuento y habilita ingresar la cantidad de productos.

	   3.3 El cajero indica la cantidad del producto.

	   3.4 El sistema agrega el/los productos, actualiza el subtotal y el total.

	4. El sistema muestra el importe a pagar aplicando los descuentos/promociones.

	5. El cajero selecciona 'Realizar pago' y genera un codigo QR

	6. El cliente escanea el codigo QR e inicia el caso de uso: Realizar Pago

	7. El sistema recibe la autorizacion del pago

	8. El cajero indica la cantidad de cuotas: 1, 2, 3 o 6

	9. El sistema registra las cuotas,finaliza la venta, imprime el ticket y finaliza el caso de uso.

## Post Condicion

Venta registrada y actuliza el stock del producto con el sistema de stock

## Flujo alternativo

En cualquier momento antes del paso 4 (¿5?), el actor puede cancelar el caso de uso.

### A0. (TA inexistente)

	1.1  [Se extiende del CU: Generar TA]

	1.2  Continua en el punto 2 del flujo normal

### A1. (Codigo de barras no reconocido)

	3.1.1 El sistema muestra un mensaje: "Codigo de barras invalido"

	3.1.2 El cajero ingresa el codigo manualmente.
	
	3.1.3 Continua en el punto 3.2 del flujo normal

### A2. (Codigo invalido, ingrese nuevamente el codigo)

	3.1.3.1 El sistema muestra un mensaje: "Codigo ingresado manualmente no valido" 

	3.1.3.2 Continua en el punto 3.1.2 del flujo normal

### A3. (Pago rechazado)

	7.1 El sistema muestra un mensaje: "Pago rechazado" 

	7.2 Continua en el punto 6 del flujo normal
