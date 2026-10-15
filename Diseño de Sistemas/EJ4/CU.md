# Nombre CU: Realizar Compra
# Actor Principal: Cliente
# Objetivo: Registrar una compra de productos, especificando el metodo de envio y emitiendo la factura de la compra
# Precondiciones: Iniciar Sesion, Agregar Producto
# Puntos de Extension: Registrar Domicilio
# Flujo Principal
1. El cliente presiona en "Realizar Compra" e inicia el caso de uso
2. El sistema muestra la interfaz con fecha y hora, carga y muestra las opciones de entrega
3. El cliente selecciona la opcion de entrega "Retiro en sucursal".
4. El sistema carga y muestra las sucursales disponibles 
5. El cliente selecciona la sucursal para retirar la compra
6. El sistema carga los datos personales del cliente (Nombre, Apellido, Mail).
7. El cliente ingresa los datos de la tarjeta de credito y presiona en "Pagar"
8. El sistema registra la compra, procesa el pago y emite una factura con un codigo de retiro
# Flujo Alternativo
    *En cualquier momento antes del paso  7, el actor puede cancelar el CU*
    A1. [Entrega a domicilio]
    3.1 El cliente selecciona la opcion "Entrega a domicilio"
    3.2 El sistema carga las direcciones del cliente
    3.3 El cliente selecciona la direccion que desea
    3.4 Vuelve al paso 5 del flujo normal
    A1.1. [Nuevo Domicilio]
    3.3.1 Se extiende del CU de Registrar Domicilio
    3.3.2 Vuelve al paso 3.2 del flujo A1
    A2. [Error en el pago]
    6.1 El sistema muestra un mensaje de error "Error al confirmar el pago"
    6.2 Vuelve al paso 7 del flujo normal


