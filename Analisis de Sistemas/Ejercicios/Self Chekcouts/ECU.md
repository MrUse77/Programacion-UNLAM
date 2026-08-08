# CU: Realizar Compra

## Tipo de CU

Base

## Objetivo del CU

El cliente puede realizar una compra, seleccionando el tipo de comprador, pesando sus productos y realizando el pago.

## Actor Principal

Cliente

## Actor secundario

Sensor

## Autor

Agustin

## Fecha de creacion

13 de octubre del 2025

## Precondicion

Registrar Producto, Registrar Tipo de Comprador, Registrar Caja

## Puntos de extension

No aplica

## Flujo Normal

1. El cliente oprime en "Realizar compra" e inicia el caso de uso.
2. El sistema muestra la fecha y la hora.
3. El cliente selecciona el tipo de comprador (Cliente frecuente o Cliente nuevo).
4. El sistema muestra el tipo de comprador seleccionado y habilita el lector.
5. Por cada Producto a comprar:
   5.1. El cliente escanea el código de barras del producto.
   5.2. El sistema habilita la balanza en la zona de empaque.
   5.3. El cliente coloca el producto en la balanza.
   5.4. El sistema registra el peso, el precio del producto y actualiza el subtotal.
6. El cliente selecciona "Continuar" para proceder al pago.
7. El sistema muestra el total a pagar.
8. El cliente selecciona el método de pago (Efectivo o Tarjeta).
9. El sistema procesa el pago, emite el comprobante de compra y finaliza el CU.

## Flujo alternativo

- Se podra finalizar el CU en cualquier paso del flujo normal antes del paso 6.

A0.[Producto no registrado]
5.1.1. El sistema muestra un mensaje: "Producto no registrado".
5.1.2. Fin del CU
