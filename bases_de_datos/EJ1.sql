

use Ejercicios;
/*Creacion de la base de datos EJ1*/
/*
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

/*insert into EJ1.Proveedor (CodProv,Nombre,Domicilio, Ciudad) values (1,'Proveedor A', 'Calle 1', 'La Plata');
insert into EJ1.Proveedor (CodProv,Nombre, Domicilio, Ciudad) values (2,'Proveedor B', 'Calle 1', 'Haedo');
insert into EJ1.Proveedor (CodProv,Nombre, Domicilio, Ciudad) values (3,'Proveedor C', 'Calle 1', 'La Plata');*/
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

/***** 4 litar los codigos de los materiales que provea el proveedor 10 y no el proveedor 15 *****/

/*update EJ1.Proveedor set CodProv = 10 where Nombre = 'Proveedor A';
update EJ1.Proveedor set CodProv = 15 where Nombre = 'Proveedor B';
update EJ1.Proveedor set CodProv = 10 where Nombre = 'Proveedor C';
insert into EJ1.Material (CodMat,descripcion) values (1,'Material A');
insert into EJ1.Material (CodMat,descripcion) values (2,'Material B');
insert into EJ1.Material (CodMat,descripcion) values (3,'Material C');
insert into EJ1.Material (CodMat,descripcion) values (4,'Material D');
insert into EJ1.Provisto_Por (CodMat, CodProv) values (1, 10);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (2, 10);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (3, 15);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (4, 15);
insert into EJ1.Provisto_Por (CodMat, CodProv) values (3, 10);*/
select CodMAt as p10 from (
  select CodMat from EJ1.Provisto_Por where CodProv = 10
) as t1 where CodMat not in (
  select CodMat from EJ1.Provisto_Por where CodProv = 15
);


