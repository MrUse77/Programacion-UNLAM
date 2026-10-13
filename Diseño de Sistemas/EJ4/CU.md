# Nombre CU: Realizar Compra
# Actor Principal: Cliente
# Objetivo: Registrar una compra de productos, especificando el metodo de envio y emitiendo la factura de la compra
# Precondiciones: Iniciar Sesion, Agregar Producto, Registrar Direccion
# Puntos de Extension: Registrar Domicilio
# Flujo Principal
1. El cliente presiona en "Realizar Compra" e inicia el caso de uso
2. El sistema muestra la interfaz con fecha y hora, carga y muestra las opciones de entrega
3. El cliente selecciona la opcion de entrega "Entrega en sucursal".
4. El sistema carga los datos personales del cliente (Nombre, Apellido, Mail).
5. El cliente ingresa los datos de la tarjeta de credito y presiona en "Pagar"
6. El sistema registra la compra, procesa el pago y emite una factura con un codigo de retiro
# Flujo Alternativo
    *En cualquier momento antes del paso  5, el actor puede cancelar el CU*
    A1. [Entrega a domicilio]
    3.1 El cliente selecciona la opcion "Entrega a domicilio"
    3.2 El sistema carga las direcciones del cliente
    3.3 El cliente selecciona la direccion que desea
    3.4 Vuelve al paso 4 del flujo normal
    A2. [Error en el pago]
    6.1 El sistema muestra un mensaje de error "Error al confirmar el pago"
    6.2 Vuelve al paso 4 del flujo normal


