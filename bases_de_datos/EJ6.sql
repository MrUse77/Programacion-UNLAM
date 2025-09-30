use Ejercicios;

create schema EJ6;

create table EJ6.vuelo(
	nroVuelo int not null,
	desde varchar(50) not null,
	hasta varchar(50) not null,
	primary key(nroVuelo) 
)

create table EJ6.avion_utilizado(
	nroVuelo int not null,
	tipoAvion varchar(50) not null,
	nroAvion varchar(50) not null,
	PRIMARY KEY(nroVuelo)
	constraint FKVueloAvion FOREIGN KEY(nroVuelo) REFERENCES EJ6.vuelo(nroVuelo)
)

create table EJ6.info_pasajeros(
	nroVuelo int not null
	documento int not null,
	nombre varchar(50) not null,
	origen varchar(50) not null,
	destino varchar(50) not null,
	PRIMARY KEY(nroVuelo, documento)
	constraint FKVuelo FOREIGN KEY(nroVuelo) REFERENCES EJ6.vuelo(nroVuelo)
)

---- 1. Hallar los números de vuelo desde el origen A hasta el destino F.
select nroVuelo from EJ6.vuelo v where v.desde='A' and v.hasta='F' 

---- 2. Hallar los tipos de avión que no son utilizados en ningún vuelo que pase por B.
select au.tipoAvion from EJ6.avion_utilizado a where not exists(
	select 1 from EJ6.vuelo v inner join EJ6.info_pasajeros i on v.nroVuelo = i.nroVuelo 
	where (v.desde = 'B' or v.hasta= 'H'i.origen = 'B' or i.destino = 'B') and v.nroVuelo = a.nroVuelo
)

---- 3. Hallar los pasajeros y números de vuelo para aquellos pasajeros que viajan desde A a D pasando por B.
-
