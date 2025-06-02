
/*Creacion de la base de datos EJ1*/
/*
 * use Ejercicios
 * create schema EJ1;
create table EJ1.Almacen (
  Nro int primary key,
  Responsable varchar(50) not null
);

create table EJ1.Articulo(
  CodArt int primary key ,
  descripcion varchar(50) not null,
  precio int not null
);

create table EJ1.Material(
  CodMat int primary key,
  descripcion varchar(50) not null
  precio float not null,
);

create table EJ1.Proveedor(
  CodProv int primary key,
  Nombre varchar(50) not null,
  Domicilio varchar(50) not null,
  Ciudad varchar(50) not null
);

CREATE TABLE EJ1.Tiene (
  CodArt INT NOT NULL,
  Nro INT NOT NULL,
  CONSTRAINT pk_tiene PRIMARY KEY (CodArt, Nro),
  CONSTRAINT fk_tiene_almacen FOREIGN KEY (Nro) REFERENCES EJ1.Almacen(Nro),
  CONSTRAINT fk_tiene_articulo FOREIGN KEY (CodArt) REFERENCES EJ1.Articulo(CodArt)
);

CREATE TABLE EJ1.Compuesto_Por (
  CodArt INT NOT NULL,
  CodMat INT NOT NULL,
  CONSTRAINT pk_compuesto_por PRIMARY KEY (CodArt, CodMat),
  CONSTRAINT fk_cp_articulo FOREIGN KEY (CodArt) REFERENCES EJ1.Articulo(CodArt),
  CONSTRAINT fk_cp_material FOREIGN KEY (CodMat) REFERENCES EJ1.Material(CodMat)
);

CREATE TABLE EJ1.Provisto_Por (
  CodMat INT NOT NULL,
  CodProv INT NOT NULL,
  CONSTRAINT pk_provisto_por PRIMARY KEY (CodMat, CodProv),
  CONSTRAINT fk_pp_articulo FOREIGN KEY (CodMat) REFERENCES EJ1.Material(CodMat),
  CONSTRAINT fk_pp_proveedor FOREIGN KEY (CodProv) REFERENCES EJ1.Proveedor(CodProv)
);
*/

/***** 1 Listar los nombres de proveedores de La Plata *****/

/*
insert into EJ1.Proveedor (CodProv,Nombre,Domicilio, Ciudad) values (7,'Chéco Pérez', 'Calle 1', 'Pergamino');
insert into EJ1.Proveedor (CodProv,Nombre, Domicilio, Ciudad) values (9,'Lopez', 'Calle 1', 'Pergamino');
insert into EJ1.Proveedor (CodProv,Nombre, Domicilio, Ciudad) values (3,'Gomez', 'Calle 1', 'La Plata');
insert into EJ1.Proveedor (CodProv,Nombre,Domicilio, Ciudad) values (8,'Sergio Pérez', 'Calle 1', 'Mexico');
 */
select Nombre from EJ1.Proveedor where Ciudad = 'La Plata'; 

/***** 2 Listar los artículos cyuo precio sea inferior a $10 *****/

/*insert into EJ1.Articulo (CodArt,descripcion, precio) values (1,'Articulo A', 10);
insert into EJ1.Articulo (CodArt,descripcion, precio) values (2,'Articulo B', 4);
insert into EJ1.Articulo (CodArt,descripcion, precio) values (3,'Articulo C', 12);
insert into EJ1.Articulo (CodArt,descripcion, precio) values (4,'Articulo D',  1);*/

select CodArt from EJ1.Articulo where precio < 10;

/***** 3 Listar a los responsables de los almacenes *****/

/*insert into EJ1.Almacen (Nro,Responsable) values (1,'Responsable A');
insert into EJ1.Almacen (Nro,Responsable) values (2,'Responsable B');
insert into EJ1.Almacen (Nro,Responsable) values (3,'Responsable C');*/

select Responsable from EJ1.Almacen;

-- 4 listar los codigos de los materiales que provea el proveedor 10 y no el proveedor 15

/*
insert into EJ1.Material (CodMat,descripcion) values (1,'Material A', 120);
insert into EJ1.Material (CodMat,descripcion) values (2,'Material B', 90);
insert into EJ1.Material (CodMat,descripcion) values (3,'Material C',150);
insert into EJ1.Material (CodMat,descripcion) values (4,'Material D',200);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (1, 10);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (2, 10);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (3, 15);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (4, 15);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (3, 10);
 */
select CodMAt as p10 from (
  select CodMat from EJ1.Provisto_Por where CodProv = 10
) as t1 where CodMat not in (
  select CodMat from EJ1.Provisto_Por where CodProv = 15
);

-- 5 Listar los números de almacenes que almacenan el artículo A
/*
insert into EJ1.Tiene (CodArt, Nro) values (1, 1);
insert into EJ1.Tiene (CodArt, Nro) values (1, 2);
insert into EJ1.Tiene (CodArt, Nro) values (2, 1);
insert into EJ1.Tiene (CodArt, Nro) values (3, 3);
 */


select Nro from EJ1.tiene where CodArt in (select CodArt from EJ1.Articulo where descripcion = 'Articulo A');

-- 6 Listar los proveedores de Pergamino que se llamen Pérez.
select * from EJ1.Proveedor where Ciudad = 'Pergamino' and Nombre like '%Pérez%';


-- 7 Listar los almacenes que contienen los artículos A(cod = 1) y los artículos B(cod = 2) (ambos).
select * from EJ1.Almacen where nro = (
  select Nro  from EJ1.Tiene where CodArt = 1
  intersect
  select Nro from EJ1.Tiene where CodArt = 2
)

-- 8 Listar los artículos que cuesten más de $100 o que estén compuestos por el material M1.
/*
insert into EJ1.Compuesto_Por (CodArt, CodMat) values (1, 1);
insert into EJ1.Compuesto_Por (CodArt, CodMat) values (1, 2);
insert into EJ1.Compuesto_Por (CodArt, CodMat) values (2, 2);
insert into EJ1.Compuesto_Por (CodArt, CodMat) values (2, 1);
*/
select * from EJ1.Articulo where precio > 100 or codArt in (select codArt from EJ1.Compuesto_Por where codArt = 1);

-- 9 Listar los materiales, código y descripción, provistos por proveedores de la ciudad de Rosario.

select * from EJ1.Material where codMat in (select codMat from EJ1.Provisto_Por where CodProv in (select codProv from EJ1.Proveedor where Ciudad = 'Rosario') )

-- 10 Listar el código, descripción y precio de los artículos que se almacenan en A1

select * from EJ1.Articulo where codArt in (select codArt from EJ1.Tiene where Nro = 1)

-- 11 Listar la descripción de los materiales que componen el artículo B (cod = 2).

select descripcion from EJ1.Material where codMat in (select codMat from EJ1.Compuesto_Por where codArt = 2);

-- 12 Listar los nombres de los proveedores que proveen los materiales al almacén que Martín Gómez tiene a su cargo

select Nombre from EJ1.Proveedor where codProv in(select codProv from EJ1.Provisto_Por where codMat in (select codMat from EJ1.Compuesto_Por where CodArt in(select CodArt from EJ1.Tiene where Nro in (select Nro from EJ1.Almacen where Responsable = 'Martín Gómez'))));

-- 13 Listar códigos y descripciones de los artículos compuestos por al menos un material provisto por el proveedor López.

select CodArt, descripcion from EJ1.Articulo where CodArt in (select CodArt from EJ1.Compuesto_Por where CodMat in (select CodMat from EJ1.Provisto_Por where CodProv in (select CodProv from EJ1.Proveedor where Nombre = 'Lopez')));

-- 14 Hallar los códigos y nombres de los proveedores que proveen al menos un material que se usa en algún artículo cuyo precio es mayor a $100.

select CodProv, Nombre from EJ1.Proveedor where CodProv in (select CodProv from EJ1.Provisto_Por where CodMat in (select CodMat from EJ1.Compuesto_Por where CodArt in (select CodArt from EJ1.Articulo where precio>100)));

-- 17 Listar el/los artículo/s de mayor precio.

select CodArt, descripcion, precio from EJ1.Articulo where precio = (select max(precio) from EJ1.Articulo);

-- 18 Listar el/los articulo/s de menor precio

select CodArt, descripcion, precio from EJ1.Articulo where precio = (select min(precio) from EJ1.Articulo);

-- 19 Listar el/los el precio promedio de los articulos por almacen.

select t.Nro as Almacen, avg(ar.precio) as PrecioPromedio from EJ1.Tiene t JOIN EJ1.Articulo ar ON t.CodArt = ar.CodArt GROUP BY t.Nro;

-- 20 Listar los almacenes que almacenan la mayor cantidad de artículos.

select * from EJ1.Almacen where Nro in (select t.Nro from EJ1.Tiene t GROUP BY t.Nro HAVING count(t.CodArt) = (select max(cantidad) from (select count(CodArt) as cantidad from EJ1.Tiene group by Nro) as subquery));

-- 21 Listar los articulos compuesto por al menos 2 materiales.

select * from Ej1.Articulo where CodArt in (select CodArt from EJ1.Compuesto_Por group by CodArt having count(CodMat) >= 2);

-- 22 Listar los articulos compuesto por exactamente 2 materiales.

select * from Ej1.Articulo where CodArt in (select CodArt from EJ1.Compuesto_Por group by CodArt having count(CodMat) = 2);

-- 23 LIstar los articulos compuesto con hasta 2 materiales.

select * from Ej1.Articulo where CodArt in (select CodArt from EJ1.Compuesto_Por group by CodArt having count(CodMat) <= 2);

-- 24 Listar los articulos compuestos por todos los materiales.

select * from EJ1.articulo where CodArt in (select CodArt from EJ1.Compuesto_Por group by CodArt having count(CodMat) = (select count(*) from EJ1.Material));

-- 25 Listar todas las ciudades de los proveedores que provean todos los materiales.

select Ciudad from EJ1.Proveedor where CodProv in (select CodProv from EJ1.Provisto_Por group by CodProv having count(CodMat) = (select count(*) from EJ1.Material));


select t.CodArt , t.Nro from EJ1.Tiene t group by t.CodArt;


-- Precio potencial si todos los proveedores suministraran todos los materiales
SELECT p.Nombre as Nproveedor, m.descripcion AS Dmaterial,
       12 AS precio_con_ganancia
FROM EJ1.Proveedor p
CROSS JOIN EJ1.Material m
WHERE NOT EXISTS (
    SELECT 1 FROM EJ1.Provisto_por pp 
    WHERE pp.CodProv = p.CodProv AND pp.CodMat = m.CodMat
);


