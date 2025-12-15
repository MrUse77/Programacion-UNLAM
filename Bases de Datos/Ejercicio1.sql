/*
 Dada la siguiente base de datos:
Almacén (Nro, Responsable)
Articulo (CodArt, descripción, Precio)
Material (CodMat, Descripción)
Proveedor (CodProv, Nombre, Domicilio, Ciudad)
Tiene (Nro, CodArt)
Compuesto_por (CodArt, CodMat)
Provisto_por (CodMat, CodProv)
*/
use ej1_ArticulosDeAlmacen

/* 1. Listar los nombres de los proveedores de la ciudad de La Plata. */
SELECT Nombre FROM Proveedor WHERE Ciudad = 'La Plata';

/*2. Listar los números de artículos cuyo precio sea inferior a $10.*/
SELECT CodArt FROM Articulo WHERE Precio < 10;

/*3. Listar los responsables de los almacenes.*/
SELECT Responsable FROM Almacen;

/*4. Listar los códigos de los materiales que provea el proveedor 10 y no los provea el proveedor 15.*/
SELECT CodMat FROM Provisto_por WHERE CodProv = 10 AND CodMat NOT IN (SELECT CodMat FROM Provisto_por WHERE CodProv = 15);

/*5. Listar los números de almacenes que almacenan el artículo A (1).*/
SELECT Nro FROM Tiene WHERE CodArt = 2;

/*6. Listar los proveedores de Pergamino que se llamen Pérez.*/
SELECT Nombre FROM Proveedor WHERE Ciudad = 'Pergamino' AND Nombre = 'Pérez';

/*7. Listar los almacenes que contienen los artículos A(1) y los artículos B(2) (ambos).*/
SELECT Nro FROM Tiene WHERE CodArt = 1 INTERSECT SELECT Nro FROM Tiene WHERE CodArt = 2;

/*8. Listar los artículos que cuesten más de $100 o que estén compuestos por el material M1.*/
SELECT CodArt FROM Articulo WHERE Precio > 100 UNION SELECT CodArt FROM Compuesto_por WHERE CodMat = 1;

/*9. Listar los materiales, código y descripción, provistos por proveedores de la ciudad de Rosario.*/
SELECT CodMat, Descripcion FROM MATERIAL WHERE CodMat IN(SELECT CodMat FROM Provisto_por WHERE CodProv IN (SElECT CodProv FROM Proveedor WHERE Ciudad = 'Rosario'));

/*10. Listar el código, descripción y precio de los artículos que se almacenan en A1(1).*/
SELECT a.CodArt, a.Descripcion, a.Precio FROM Articulo a JOIN Tiene t ON a.CodArt = t.CodArt WHERE t.Nro = 1;

/*11. Listar la descripción de los materiales que componen el artículo B(2).*/
SELECT m.Descripcion FROM Material m JOIN Compuesto_por cp ON m.CodMat = cp.CodMat WHERE cp.CodArt = 2;

/*12. Listar los nombres de los proveedores que proveen los materiales al almacén que Martín Gómez tiene a su cargo.*/
SELECT DISTINCT p.Nombre FROM Proveedor p JOIN Provisto_por pp ON p.CodProv = pp.CodProv JOIN Compuesto_por cp ON pp.CodMat = cp.CodMat JOIN Tiene t ON cp.CodArt = t.CodArt JOIN Almacen a ON t.Nro = a.Nro WHERE a.Responsable = 'Martín Gómez';

/*13. Listar códigos y descripciones de los artículos compuestos por al menos un material provisto por el proveedor López.*/
SELECT DISTINCT a.CodArt, a.Descripcion FROM Articulo a JOIN Compuesto_por cp ON a.CodArt = cp.CodArt JOIN Provisto_por pp ON cp.CodMat = pp.CodMat JOIN Proveedor p ON pp.CodProv = p.CodProv WHERE p.Nombre = 'López';

/*14. Hallar los códigos y nombres de los proveedores que proveen al menos un material que se usa en algún artículo cuyo precio es mayor a $100.*/
SELECT DISTINCT p.CodProv, p.Nombre FROM Proveedor p JOIN Provisto_por pp ON p.CodProv = pp.CodProv JOIN Compuesto_por cp ON pp.CodMat = cp.CodMat JOIN Articulo a ON cp.CodArt = a.CodArt WHERE a.Precio > 100;

/*15. Listar los números de almacenes que tienen todos los artículos que incluyen el material con código 123.*/
SELECT t.Nro FROM Tiene t JOIN Compuesto_por cp ON t.CodArt = cp.CodArt WHERE cp.CodMat = 123 GROUP BY t.Nro HAVING COUNT(DISTINCT t.CodArt) = (SELECT COUNT(DISTINCT CodArt) FROM Compuesto_por WHERE CodMat = 123);

/*16. Listar los proveedores de Capital Federal que sean únicos proveedores de algún material.*/
SELECT p.CodProv, p.Nombre FROM Proveedor p JOIN Provisto_por pp ON p.CodProv = pp.CodProv GROUP BY pp.CodMat HAVING COUNT(pp.CodProv) = 1 AND p.Ciudad = 'Capital Federal';

/*17. Listar el/los artículo/s de mayor precio.*/
SELECT * FROM Articulo WHERE Precio = (SELECT MAX(Precio) FROM Articulo);

/*18. Listar el/los artículo/s de menor precio.*/
SELECT * FROM Articulo WHERE Precio = (SELECT MIN(Precio) FROM Articulo);

/*19. Listar el promedio de precios de los artículos en cada almacén.*/
SELECT t.Nro, AVG(a.Precio) AS Promedio_Precios FROM Tiene t JOIN Articulo a ON t.CodArt = a.CodArt GROUP BY t.Nro;

/*20. Listar los almacenes que almacenan la mayor cantidad de artículos.*/
SELECT t.Nro FROM Tiene t GROUP BY t.Nro HAVING COUNT(t.CodArt) = (SELECT MAX(Articulos_Almacen) FROM (SELECT COUNT(CodArt) AS Articulos_Almacen FROM Tiene GROUP BY Nro) AS Subconsulta);

/*21. Listar los artículos compuestos por al menos 2 materiales.*/
SELECT CodArt FROM Compuesto_por GROUP BY CodArt HAVING COUNT(CodMat) >= 2;

/*22. Listar los artículos compuestos por exactamente 2 materiales.*/
SELECT CodArt FROM Compuesto_por GROUP BY CodArt HAVING COUNT(CodMat) = 2;

/*23. Listar los artículos que estén compuestos con hasta 2 materiales.*/
SELECT CodArt FROM Compuesto_por GROUP BY CodArt HAVING COUNT(CodMat) <= 2;

/*24. Listar los artículos compuestos por todos los materiales.*/
SELECT CodArt FROM Compuesto_por GROUP BY CodArt HAVING COUNT(CodMat) = (SELECT COUNT(*) FROM Material);

/*25. Listar las ciudades donde existan proveedores que provean todos los materiales.*/
SELECT DISTINCT p.Ciudad FROM Proveedor p JOIN Provisto_por pp ON p.CodProv = pp.CodProv GROUP BY p.Ciudad HAVING COUNT(DISTINCT pp.CodMat) = (SELECT COUNT(*) FROM Material);









