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

1. El cajero selecciona Registrar Venta e inicia el caso de uso.

2. El sistema muesta la interfaz con fecha, hora y nombre del cajero.

3. El cajero ingresa la tarjeta de afinidad.

4. El sistema muestra los datos del cliente y habilita el lector.

5. Por cada producto:

   5.1 El cajero indica la cantidad de productos y pasa el codigo de barras del producto por el lector.

   5.2 El sistema muestra el nombre del producto, codigo, precio y actualiza el total.

6. El cajero pulsa "Fin carga de productos"

7. El sistema muestra el importe a pagar aplicando los descuentos/promociones.

8. El cajero selecciona 'Realizar pago' y genera un codigo QR

9. El cliente escanea el codigo QR
   [Se incluye comunicacion con la aplicacion bancaria]

10. El sistema recibe la autorizacion del pago

11. El cajero indica la cantidad de cuotas: 1, 2, 3 o 6

12. El sistema registra las cuotas,finaliza la venta, imprime el ticket y finaliza el caso de uso.

## Post Condicion

Venta registrada y actuliza el stock del producto con el sistema de stock

## Flujo alternativo

En cualquier momento antes del paso 6, el actor puede cancelar el caso de uso.

### A0. (TA inexistente)

1.1 [Se extiende del CU: Generar TA]

1.2 Continua en el punto 2 del flujo normal

### A1. (Codigo de barras no reconocido)

5.1.1 El sistema muestra un mensaje: "Codigo de barras invalido"

5.1.2 El cajero ingresa el codigo manualmente.

5.1.3 Continua en el punto 3.2 del flujo normal

### A2. (Codigo invalido, ingrese nuevamente el codigo)

5.1.3.1 El sistema muestra un mensaje: "Codigo ingresado manualmente no valido"

5.1.3.2 Continua en el punto 4.1.2 del flujo normal

### A3. (Pago rechazado)

7.1 El sistema muestra un mensaje: "Pago rechazado"

7.2 Continua en el punto 6 del flujo normal
