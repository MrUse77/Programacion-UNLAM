/*Dada la siguiente base de datos:
Proveedor (NroProv, NomProv, Categoria, CiudadProv)
Artículo (NroArt, Descripción, CiudadArt, Precio)
Cliente (NroCli, NomCli, CiudadCli)
Pedido (NroPed, NroArt, NroCli, NroProv, FechaPedido,
Cantidad, PrecioTotal)
Stock (NroArt, fecha, cantidad)
*/
CREATE TABLE Proveedor (
    NroProv CHAR(5) PRIMARY KEY,
    NomProv VARCHAR(100),
    Categoria VARCHAR(50),
    CiudadProv VARCHAR(50)
);
CREATE TABLE Artículo (
    NroArt CHAR(5) PRIMARY KEY,
    Descripción VARCHAR(255),
    CiudadArt VARCHAR(50),
    Precio DECIMAL(10, 2)
);
CREATE TABLE Cliente (
    NroCli CHAR(5) PRIMARY KEY,
    NomCli VARCHAR(100),
    CiudadCli VARCHAR(50)
);
CREATE TABLE Pedido (
    NroPed CHAR(5) PRIMARY KEY,
    NroArt CHAR(5),
    NroCli CHAR(5),
    NroProv CHAR(5),
    FechaPedido DATE,
    Cantidad INT,
    PrecioTotal DECIMAL(10, 2),
    FOREIGN KEY (NroArt) REFERENCES Artículo (NroArt),
    FOREIGN KEY (NroCli) REFERENCES Cliente (NroCli),
    FOREIGN KEY (NroProv) REFERENCES Proveedor (NroProv)
);
CREATE TABLE Stock (
    NroArt CHAR(5),
    fecha DATE,
    cantidad INT,
    PRIMARY KEY (NroArt, fecha), FOREIGN KEY (NroArt) REFERENCES Artículo (NroArt)
);

/*1. Hallar el código (nroProv) de los proveedores que proveen el artículo a146.*/
--Es posible usar EXISTS
SELECT DISTINCT p.nroProv FROM Proveedor p JOIN Pedido pe ON pe.nroProv = p.nroProv WHERE pe.nroArt = 'a146';

/*2. Hallar los clientes (nomCli) que solicitan artículos provistos por p015.*/
--Es posible usar EXISTS
SELECT DISTINCT c.NomCli FROM Pedido p JOIN Cliente c ON p.NroCli = c.NroCli WHERE p.NroProv='p015';

/*3. Hallar los clientes que solicitan algún item provisto por proveedores con categoría mayor que 4.*/
SELECT * FROM Cliente c WHERE EXISTS (SELECT pe.NroCli FROM Pedido pe INNER JOIN Proveedor p ON pe.NroProv = p.NroProv WHERE pe.NroCli = c.Cli AND p.categoria > 4);

/*4. Hallar los pedidos en los que un cliente de Rosario solicita artículos producidos en la ciudad de Mendoza.*/
SELECT * FROM Pedido pe
JOIN Artículo a ON pe.NroArt = a.NroArt
JOIN Cliente c ON pe.NroCli = c.NroCli
WHERE a.CiudadArt = 'Mendoza' AND c.CiudadCli = 'Rosario'; 

/*5. Hallar los pedidos en los que el cliente c23 solicita artículos solicitados por el cliente c30*/
SELECT pe.* FROM Pedidio pe
WHERE pe.NroCli = 'c23' AND EXISTS(
	SELECT 1 FROM Pedido pe1 
	WHERE pe1.NroCli = 'c30'
	AND pe1.NroArt = pe1.NroArt
);

/*6. Hallar los proveedores que suministran todos los artículos cuyo precio es superior
al precio promedio de los artículos que se producen en La Plata.*/
WITH prom_lp AS (
  SELECT AVG(a2.Precio) AS prom
  FROM Articulo a2
  WHERE a2.CiudadArt = 'La Plata'
)
SELECT p.NroProv AS Proveedor
FROM Proveedor p
WHERE NOT EXISTS (
  SELECT 1
  FROM Articulo a
  CROSS JOIN prom_lp plp
  WHERE a.Precio > plp.prom              -- artículos por encima del promedio LP
    AND NOT EXISTS (                     -- que este proveedor NO suministre
      SELECT 1
      FROM Pedido pe
      WHERE pe.NroProv = p.NroProv
        AND pe.NroArt = a.NroArt
    )
);




