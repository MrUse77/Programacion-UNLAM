
-- crear base de datos
create database ej1_ArticulosDeAlmacen;

-- usar la base de datos
use ej1_ArticulosDeAlmacen;

-- crear tablas
create table almacen
(
	nro int not null,
	responsable varchar(50) not null,
	primary key(nro)
);

create table articulo
(
	codArt int not null,
	descripcion varchar(50) not null,
	precio float not null,
	primary key(codArt)
);

create table material
(
	codMat int not null,
	descripcion varchar(50) not null,
	primary key(codMat)
);

create table proveedor
(
	codProv int not null,
	nombre varchar(50) not null,
	domicilio varchar(50) not null,
	ciudad varchar(50) not null,
	primary key(codProv)
);

create table tiene
(
	nro int not null,
	codArt int not null,
	primary key(nro, codArt),
	foreign key(nro) references almacen(nro),
	foreign key(codArt) references articulo(codArt)
);

create table compuesto_por
(
	codArt int not null,
	codMat int not null,
	primary key(codArt, codMat),
	foreign key(codArt) references articulo(codArt),
	foreign key(codMat) references material(codMat)
);

create table provisto_por
(
	codMat int not null,
	codProv int not null,
	primary key(codMat, codProv),
	foreign key(codMat) references material(codMat),
	foreign key(codProv) references proveedor(codProv)
);

-- cargar registros en tabla almacen
insert into almacen(nro, responsable) values
(1, 'Chino'),
(2, 'Don Manuel'),
(3, 'Kuki'),
(4, 'Tuerto'),
(5, 'Joaquín');

select * from almacen;

-- cargar registros en tabla articulo
insert into articulo(codArt, descripcion, precio) values
(1,'Pan lactal Bimbo chico', 30),
(2,'Arvejas Marolio', 12),
(3,'Arroz Gallo extrafino', 18),
(4,'Cerveza Quilmes', 32),
(5,'Leche La Serenísima', 25);

select * from articulo;

-- cargar registros en tabla material
insert into material(codMat, descripcion) values
(1, 'Harina'),
(2, 'Levadura'),
(3, 'Leche pasteurizada'),
(4, 'Hierro'),
(5, 'Zinc');

select * from material;

-- cargar registros en tabla proveedor
insert into proveedor(codProv, nombre, domicilio, ciudad) values
(1, 'Panaderia Adrián', 'Spiro 5623', 'Laferrere'),
(2, 'Marolio', 'Artigas 5689', 'La Plata'),
(3, 'La Serenísima', 'Echeverría 6899', 'Longchamps'),
(4, 'Quilmes', 'Figueroa 4744', 'Quilmes'),
(5, 'Bimbo', 'Lautaro Gutiérrez 1657', 'Martínez'),
(6, 'Gallo', 'Puerto Mexico 2625', 'Gral. Rodríguez');

select * from proveedor;

-- cargar registros en tabla tiene
insert into tiene(nro, codArt) values
(1, 1),
(1, 2),
(1, 3),
(1, 4),
(1, 5),
(2, 2),
(2, 5),
(4, 2),
(4, 3),
(4, 5),
(5, 4),
(5, 5);

select * from tiene;

-- cargar registros en tabla compuesto_por
insert into compuesto_por(codArt, codMat) values
(1, 1),
(1, 2),
(1, 5),
(3, 3),
(5, 3),
(5, 4),
(5, 5);

select * from compuesto_por;

-- cargar registros en tabla provisto_por
insert into provisto_por(codMat, codProv) values
(1, 1),
(1, 2),
(1, 3),
(2, 2),
(3, 5);

select * from provisto_por;
