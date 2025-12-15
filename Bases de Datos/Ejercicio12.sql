/*
Producto (CodProd, Descripcion, CodProv, StockActual)
Stock (Nro, Fecha, CodProd, Cantidad)
Proveedor (CodProv, RazonSocial, FechaInicio)
Realizar las siguientes tareas utilizando lenguaje SQL:
 */
CREATE DATABASE Ejercicio12;
use Ejercicio12;
CREATE TABLE Proveedor (
    CodProv INT PRIMARY KEY,
    RazonSocial VARCHAR(255),
    FechaInicio DATE
);
CREATE TABLE Producto (
    CodProd INT PRIMARY KEY,
    Descripcion VARCHAR(255),
    CodProv INT,
    StockActual INT,
    FOREIGN KEY (CodProv) REFERENCES Proveedor (CodProv)
);
CREATE TABLE Stock (
    Nro INT PRIMARY KEY,
    Fecha DATE,
    CodProd INT,
    Cantidad INT,
    FOREIGN KEY (CodProd) REFERENCES Producto (CodProd)
);

/*1. p_EliminaSinstock(): Realizar un procedimiento que elimine los productos que no poseen stock.*/
CREATE PROCEDURE p_EliminaSinstock AS
BEGIN
DELETE FROM Producto
WHERE StockActual = 0 ;
END ;

/*2. p_ActualizaStock(): Para los casos que se presenten inconvenientes en los datos,
se necesita realizar un procedimiento que permita actualizar todos los Stock_Actual
de los productos, tomando los datos de la entidad Stock. Para ello, se utilizará como
stock válido la última fecha en la cual se haya cargado el stock.*/
CREATE PROCEDURE p_ActualizaStock  AS
BEGIN
UPDATE Producto P
SET P.StockActual = (
SELECT S.Cantidad
FROM Stock S
WHERE S.CodProd = P.CodProd
ORDER BY S.Fecha DESC
LIMIT 1
)
END;


/*3. p_DepuraProveedor(): Realizar un procedimiento que permita depurar todos los
proveedores de los cuales no se posea stock de ningún producto provisto desde hace más de 1 año.*/
CREATE PROCEDURE p_DepuraProveedor AS
BEGIN
DELETE FROM Proveedor P
WHERE NOT EXISTS (
SELECT 1
FROM Producto PR
JOIN Stock S ON PR.CodProd = S.CodProd
WHERE PR.CodProv = P.CodProv
AND S.Fecha > = DATEADD (YEAR, - 1, GETDATE ())
) ;
END ;

/*4. p_InsertStock(nro,fecha,prod,cantidad): Realizar un procedimiento que
permita agregar stocks de productos. Al realizar la inserción se deberá validar que:
	a. El producto debe ser un producto existente
	b. La cantidad de stock del producto debe ser cualquier número entero mayor a cero.
	c. El número de stock será un valor correlativo que se irá agregando por cada nuevo stock de producto.*/
CREATE PROCEDURE p_InsertStock (@nro INT, @fecha DATE, @prod INT, @cantidad INT) AS
	BEGIN
		DECLARE @maxNro INT;
		SET @maxNro = (SELECT MAX(Nro) FROM Stock) + 1;
		IF EXISTS (SELECT 1 FROM Producto WHERE CodProd = @prod) AND @cantidad > 0 AND @nro = @maxNro
			BEGIN
				INSERT INTO Stock (Nro, Fecha, CodProd, Cantidad)
				VALUES (@nro, @fecha, @prod, @cantidad);
			END
		ELSE
			BEGIN
				RAISERROR('Error: Producto no existe, cantidad invalida o numero de stock incorrecto.', 16, 1);
			END
	END;

/*5. tg_CrearStock: Realizar un trigger que permita automáticamente agregar un
registro en la entidad Stock, cada vez que se inserte un nuevo producto. El stock
inicial a tomar, será el valor del campo Stock_Actual.*/
CREATE TRIGGER tg_CrearStock  ON Producto AFTER INSERT AS
	BEGIN 
		INSERT INTO Stock (Nro, Fecha, CodProd, Cantidad)
		SELECT 
			(SELECT ISNULL(MAX(Nro), 0) + 1 FROM Stock) AS Nro,
			GETDATE() AS Fecha,
			i.CodProd,
			i.StockActual
		FROM inserted i;
	END;

/*6. p_ListaSinStock(): Crear un procedimiento que permita listar los productos que no
posean stock en este momento y que no haya ingresado ninguno en este último
mes. De estos productos, listar el código y nombre del producto, razón social del
proveedor y stock que se tenía al mes anterior.*/
CREATE PROCEDURE p_ListaSinStock AS
	BEGIN
		SELECT P.CodProd, P.Descripcion, PR.RazonSocial, S.Cantidad FROM Producto P 
			JOIN Proveedor PR ON P.CodProv = PR.CodProv
	END;

/*7. p_ListaStock(): Realizar un procedimiento que permita generar el siguiente
reporte:
----------------------------------
Fecha      | > 1000 | < 1000 | = 0
01/08/2009 |   100  |    8   | 3
03/08/2009 |   53   |   50   | 7
04/08/2009 |   50   |   20   | 40
En este listado se observa que se contará la cantidad de productos que posean
a una determinada fecha más de 1000 unidades, menos de 1000 unidades o que
no existan unidades de ese producto.
Según el ejemplo, el 01/08/2009 existen 100 productos que poseen más de 1000
unidades, en cambio el 03/08/2009 sólo hubo 53 productos con más de 1000
unidades.*/

GO
CREATE PROCEDURE p_ListaStock AS 
	BEGIN
		SET NOCOUNT ON;
		SELECT 
			F.Fecha,
			(SELECT COUNT(*) FROM Stock S JOIN  Producto P ON S.CodProd = P.CodProd WHERE S.Fecha = F.Fecha AND P.StockActual > 1000) AS '> 1000',		
			(SELECT COUNT(*) FROM Stock S JOIN  Producto P ON S.CodProd = P.CodProd WHERE S.Fecha = F.Fecha AND P.StockActual < 1000 AND P.StockActual > 0)  AS '< 1000',
			(SELECT COUNT(*) FROM Stock S JOIN  Producto P ON S.CodProd = P.CodProd WHERE S.Fecha = F.Fecha AND P.StockActual < 1000 AND P.StockActual > 0) AS '= 0'
		FROM (SELECT DISTINCT Fecha FROM Stock) as F
		ORDER BY F.Fecha;
	END;

/*8. El siguiente requerimiento consiste en actualizar el campo stock actual de la entidad
producto, cada vez que se altere una cantidad (positiva o negativa) de ese producto.
El stock actual reflejará el stock que exista del producto, sabiendo que en la entidad
Stock se almacenará la cantidad que ingrese o egrese. Además, se debe impedir
que el campo “Stock actual” pueda ser actualizado manualmente. Si esto sucede,
se deberá dar marcha atrás a la operación indicando que no está permitido*/

