use Ejercicios;

-- crear esquema
create SCHEMA EJ2;


-- crear tablas
create table EJ2.proveedor
(
	nroProv int not null,
	nomProv varchar(50) not null,
	categoria int not null,
	ciudadProv varchar(50) not null,
	primary key(nroProv)
);

create table EJ2.articulo
(
	nroArt int not null,
	descripcion varchar(50) not null,
	ciudadArt varchar(50) not null,
	precio float not null,
	primary key(nroArt)
);

create table EJ2.cliente
(
	nroCli int not null,
	nomCli varchar(50) not null,
	ciudadCli varchar(50) not null,
	primary key(nroCli)
);

create table EJ2.pedido
(
	nroPed int not null,
	nroArt int not null,
	nroCli int not null,
	nroProv int not null,
	fechaPedido date not null,
	cantidad int not null,
	precioTotal float not null,
	primary key(nroPed),
	foreign key(nroArt) references articulo(nroArt),
	foreign key(nroCli) references cliente(nroCli),
	foreign key(nroProv) references proveedor(nroProv)
);

create table EJ2.stock
(
	nroArt int not null,
	fecha date not null,
	cantidad int not null,
	primary key(nroArt)
);

