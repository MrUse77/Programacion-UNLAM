use Ejercicios;
CREATE SCHEMA EJ3;
/***** Creacion de la base de datos EJ3 *****/

CREATE TABLE EJ3.Proveedor(
  Id_proveedor INT,
  Nombre VARCHAR(50),
  Responsabilidad_civil VARCHAR(50),
  Cuit BIGINT,
  PRIMARY KEY(id_proveedor)
);
 
CREATE TABLE EJ3.Producto(
  Id_producto INT,
  Nombre VARCHAR(50),
  Descripcion VARCHAR(50),
  Estado VARCHAR(50),
Id_proveedor INT,
  PRIMARY KEY(id_producto),
FOREIGN KEY(id_proveedor) REFERENCES EJ3.Proveedor(id_proveedor) 
);
 
CREATE TABLE EJ3.Cliente(
  Id_cliente INT,
  Nombre VARCHAR(50),
  Resp_iva VARCHAR(50),
  Cuit BIGINT,
  PRIMARY KEY(id_cliente)
);
 
CREATE TABLE EJ3.Direccion(
  Id_dir INT,
  Id_pers INT,
  Calle VARCHAR(100),
  nro INT,
  piso INT,
  dpto CHAR,
  PRIMARY KEY(id_dir),
  FOREIGN KEY(id_pers)
REFERENCES EJ3.cliente(id_cliente)
);
 
CREATE TABLE EJ3.Vendedor(
  Id_vendedor INT,
  Nombre VARCHAR(50),
  Apellido VARCHAR(50),
  Dni BIGINT,
  PRIMARY KEY(id_vendedor)
);
 
CREATE TABLE EJ3.Venta(
 Nro_factura BIGINT,
  Id_cliente INT,
  Id_vendedor INT,
  Fecha DATE ,
  PRIMARY KEY(nro_factura),
  FOREIGN KEY(id_cliente)REFERENCES EJ3.cliente(id_cliente),
  FOREIGN KEY(id_vendedor)REFERENCES EJ3.vendedor(id_vendedor)
);
 
CREATE TABLE EJ3.Detalle_venta(
  Nro_factura BIGINT,
  Nro_detalle BIGINT,
  Id_producto INT,
  Cantidad INT,
  Precio_unitario INT,
  PRIMARY KEY(nro_factura, nro_detalle),
  FOREIGN KEY(nro_factura)REFERENCES EJ3.venta(nro_factura) ON DELETE CASCADE, --borrado en cascada
  FOREIGN KEY(id_producto)REFERENCES EJ3.producto(id_producto)
);
 
---------------
CREATE TABLE EJ3.Historico_Venta(
  Nro_factura BIGINT,
  Id_cliente INT,
  Id_vendedor INT,
  Fecha DATE ,
  PRIMARY KEY(nro_factura),
  FOREIGN KEY(id_cliente)REFERENCES EJ3.Cliente(id_cliente),
  FOREIGN KEY(id_vendedor)REFERENCES EJ3.Vendedor(id_vendedor)
);
-- TRUNCATE TABLE Historico_venta;
-- DELETE FROM Historico_venta;
-- TRUNCATE TABLE Historico_Detalle_venta
CREATE TABLE EJ3.Historico_Detalle_venta(
  Nro_factura BIGINT,
  Nro_detalle BIGINT,
  Id_producto INT,
  Cantidad INT,
  Precio_unitario INT,
  PRIMARY KEY(nro_factura, nro_detalle),
  FOREIGN KEY(nro_factura)REFERENCES EJ3.historico_venta(nro_factura),
  FOREIGN KEY(id_producto)REFERENCES EJ3.producto(id_producto)
);
/* Insertar datos de prueba */
/*
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (1, 'Juan','123qw',3095513451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (2, 'Hernan','143qw',3325513451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (3, 'Sergio','163qw',30953243451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (4, 'Jesica','183qw',3095223451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (5, 'Fernando','1893qw',30946783451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (6, 'Esteban','1673qw',3095456451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (7, 'Gustavo','1793qw',3326423451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (8, 'Ruben','1676w',309234243451);
INSERT INTO EJ3.Cliente (id_cliente, nombre,resp_iva,cuit) VALUES (9, 'Ariel','18378',3097788451);
 
 
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (1, 1, 'Villegas','1780',null,null);
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (2, 1, 'Aquino','1790','3','B');
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (3, 1, 'Morlaco','1250','7','A');
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (4, 1, 'Fornuloco','8980',null,null);
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (5, 1, 'Galmarini','280','3','C');
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (6, 1, 'Leandro Alem','80',null,null);
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (7, 1, 'Santa Rosa','180','8','D');
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (8, 1, 'Budaguest','10',null,null);
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (9, 1, 'Rivadavia','3520',null,null);
INSERT INTO EJ3.Direccion (Id_dir, Id_pers, calle,nro,piso,dpto) VALUES (10, 1, 'Rolchart','2580',null,null);
 
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (1, 'Ricardo','223qw',3094413451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (2, 'Daniel','223qw',3094413451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (3, 'Ines','243qw',3324413451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (4, 'Nancy','263qw',30943243451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (5, 'Jesica','283qw',3094223451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (6, 'Fernando','2893qw',30944583451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (7, 'Esteban','2673qw',3095546451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (8, 'Gustavo','2793qw',3426423451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (9, 'Ruben','2676w',329234243451);
INSERT INTO EJ3.Proveedor (Id_proveedor, nombre,responsabilidad_civil,cuit) VALUES (10,'Ariel','28378',3297788451);
 
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (1, 1, 'goma','libreria','stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (2, 2, 'libro','libreria','sin stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (3, 3, 'hojas','libreria','stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (4, 2, 'birome','libreria','sin stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (5, 3, 'lapicera','libreria','sin stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (6, 3, 'cuaderno','libreria','stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (7, 4, 'clip','libreria','stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (8, 5, 'cartuchera','libreria','stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (9, 6, 'carpeta','libreria','stock');
INSERT INTO EJ3.Producto (Id_producto, Id_proveedor, nombre,descripcion,estado) VALUES (10,7, 'secante','libreria','stock');
 
 
INSERT INTO EJ3.Vendedor VALUES (1, 'Sergio', 'Adamoli', 35204761);
INSERT INTO EJ3.Vendedor VALUES (2, 'hernan', 'Bruno', 32204761);
INSERT INTO EJ3.Vendedor VALUES (5, 'jesica', 'Sieiro', 31204761);
INSERT INTO EJ3.Vendedor VALUES (6, 'Roberto', 'Granja', 34204761);
INSERT INTO EJ3.Vendedor VALUES (7, 'Gustavo', 'Fornica', 45204761);
INSERT INTO EJ3.Vendedor VALUES (8, 'Ariel', 'Zerpa', 25204761);
INSERT INTO EJ3.Vendedor VALUES (9, 'Rafael', 'Miceli', 55204761);
INSERT INTO EJ3.Vendedor VALUES (10, 'Daniel', 'Bustamante', 35204661);
INSERT INTO EJ3.Vendedor VALUES (11, 'Ingrid', 'Vadala', 35204561);
INSERT INTO EJ3.Vendedor VALUES (12, 'Alberto', 'Fernandez', 35204731);
SELECT * FROM EJ3.Vendedor;
 
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (1, 4,2,'2011-06-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (2, 2,2,'2011-05-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (3, 3,5,'2011-04-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (4, 1,6,'2011-03-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (5, 5,7,'2011-02-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (6, 1,8,'2011-01-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (7, 3,9,'2011-07-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (8, 4,10,'2011-08-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (9, 5,11,'2011-09-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (10, 5,15,'2019-09-26');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (11, 1,1,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (12, 1,2,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (13, 1,3,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (14, 1,4,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (15, 1,5,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (16, 1,6,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (17, 1,7,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (18, 1,8,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (19, 1,9,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (20, 1,10,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (21, 1,11,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (22, 1,12,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (23, 1,13,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (24, 1,14,'2020-06-02');
INSERT INTO EJ3.Venta (Nro_factura, id_cliente,id_vendedor,fecha) VALUES (25, 1,15,'2020-06-02');
 
 
SELECT * FROM EJ3.VENTA;
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (1, 1, 1, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (1, 2, 2, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (1, 3, 3, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (2, 1, 4, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (2, 2, 1, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (3, 1, 5, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (4, 1, 6, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (5, 1, 1, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (6, 1, 4, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (7, 1, 1, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (7, 2, 2, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (8, 1, 1, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (9, 1, 3, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (9, 2, 1, 10, 30);
INSERT INTO EJ3.Detalle_venta (Nro_factura, Nro_detalle, Id_producto, Cantidad, Precio_unitario ) VALUES (10, 2, 1, 10, 30);
*/

-- Ver que productos no se vendieron

SELECT id_producto FROM EJ3.Producto EXCEPT SELECT id_producto FROM EJ3.Detalle_Venta
--otra manera
SELECT id_producto FROM EJ3.Producto WHERE id_producto NOT IN (SELECT id_producto FROM EJ3.Detalle_Venta);

--Cantidad de unidades que funeron vendidas por producto
SELECT * from EJ3.Producto p left join EJ3.Detalle_venta dv ON p.Id_producto = dv.Id_producto;
SELECT id_producto, SUM(cantidad) AS total_vendido FROM EJ3.Detalle_Venta GROUP BY id_producto;

--Cantidad promedio de productos vendidos de cada producto
SELECT id_producto,AVG(cantidad) AS promedio from EJ3.Detalle_Venta GROUP BY id_producto;

--Vendedor que mas vendidos
SELECT v.id_vendedor, COUNT(*) AS cantidad FROM EJ3.Venta v GROUP BY v.id_vendedor;
CREATE VIEW EJ3.Vendedor_Mas_Vendido AS SELECT vta.id_vendedor, COUNT(*) AS 'CantVentas' FROM EJ3.Venta vta GROUP BY vta.id_vendedor;

--Productos con mas de 20 unidades vendidas
CREATE VIEW EJ3.Productos_Mas_Vendidos AS SELECT dv.id_producto, SUM(dv.cantidad) AS total_vendido FROM EJ3.Detalle_Venta dv GROUP BY dv.id_producto HAVING SUM(dv.cantidad) > 20;
SELECT * FROM EJ3.Productos_Mas_Vendidos;

--Clientes que compraron a todos los vendedores
CREATE VIEW EJ3.Clientes_Que_Compraron_Todos_Vendedores AS 
SELECT c.id_cliente from EJ3.Cliente c WHERE NOT EXISTS (
  SELECT 1 FROM EJ3.Vendedor v WHERE NOT EXISTS (
    SELECT 1 FROM EJ3.Venta vt WHERE vt.id_cliente = c.id_cliente AND vt.id_vendedor = v.id_vendedor
  )
);
SELECT * FROM EJ3.Clientes_Que_Compraron_Todos_Vendedores;
