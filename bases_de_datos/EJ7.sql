use Ejercicios;

create schema EJ7;

create table EJ7.auto
(
	matricula VARCHAR(12) PRIMARY KEY,
	modelo VARCHAR(50),
	anio date NOT NULL
);
create table EJ7.chofer
(
	nroLicencia BIGINT PRIMARY KEY,
	nombre VARCHAR(20) NOT NULL,
	apellido VARCHAR(20) NOT NULL,
	fechaIngreso DATE NOT NULL,
	telefono BIGINT
);
CREATE TABLE EJ7.Cliente
(
	nroCliente BIGINT PRIMARY KEY,
	calle VARCHAR(30) NOT NULL,
	nro INT NOT NULL,
	localidad VARCHAR(30)
);
create table EJ7.viaje
(
	fechaHoraInicio smalldatetime NOT NULL,
	fechaHoraFin smalldatetime ,
	chofer BIGINT NOT NULL,
	cliente BIGINT,
	auto VARCHAR(12) NOT NULL,
	kmTotales INT NOT NULL,
	esperaTotal TIME,
	costoEspera FLOAT,
	costoKms FLOAT,
	PRIMARY KEY(fechaHoraInicio, chofer), 
	FOREIGN KEY(chofer) REFERENCES EJ7.chofer(nroLicencia),
	FOREIGN KEY(cliente) REFERENCES EJ7.cliente(nroCliente),
	FOREIGN KEY(auto) REFERENCES EJ7.auto(matricula)
)

INSERT INTO EJ7.auto(matricula,modelo,anio) values
('mat1', 'mod1', '01/01/2010'), ('mat2', 'mod2', '01/01/2000'),
('mat3', 'mod3', '01/01/2001'), ('mat4', 'mod4', '01/01/2003'),
('mat5', 'mod5', '01/01/2002'), ('mat6', 'mod5', '01/01/2004'),
('mat7', 'mod6', '01/01/2006'), ('mat8', 'mod7', '01/01/2000'),
('mat9', 'mod7', '01/01/2007'), ('mat10', 'mod8', '01/01/2010');


INSERT INTO EJ7.chofer(nroLicencia, nombre, apellido, fechaIngreso, telefono) VALUES
(1, 'nom1', 'ape1', '2001-1-1', 12341234),
(2, 'nom2', 'ape2', '2002-2-1', 45674567),
(3, 'nom3', 'ape3', '2002-12-31', 98765432),
(4, 'nom4', 'ape4', '2003-2-1', 34567890),
(5, 'nom5', 'ape5', '2003-5-13', 87654321),
(6, 'nom6', 'ape6', '2003-6-5', 33333333),
(7, 'nom7', 'ape7', '2003-7-12', 44444444),
(8, 'nom8', 'ape8', '2003-8-10', 55555555),
(9, 'nom9', 'ape9', '2007-3-2', 66666666),
(10, 'nom10', 'ape10', '2009-6-18', 77777777);


INSERT INTO EJ7.cliente(nroCliente, calle,nro,localidad) VALUES
(1, 'Calle1', 1, 'loc1'),(2, 'Calle2', 2, 'loc2'),
(3, 'Calle3', 3, 'loc3'),(4, 'Calle4', 4, 'loc4'),
(5, 'Calle5', 5, 'loc5'),(6, 'Calle6', 6, 'loc6'),
(7, 'Calle7', 7, 'loc7'),(8, 'Calle8', 8, 'loc8'),
(9, 'Calle9', 9, 'loc9'),(10, 'Calle10', 10, 'loc10');



INSERT INTO EJ7.viaje(fechaHoraInicio, fechaHoraFin, chofer, cliente,auto,kmTotales,esperaTotal,costoEspera,costoKms) VALUES
('2011-01-01 00:00:00', '2011-01-01 00:30:00', 1, 1, 'mat1', 23,
'00:00:01', 5, 0.75),
('2011-01-01 00:02:00', '2011-01-01 00:22:00', 2, 1, 'mat1', 20,
'00:00:02', 10, 0.75),
('2011-01-01 01:00:00', '2011-01-01 01:30:00', 3, 3, 'mat4', 25,
'00:00:03', 15, 0.75),
('2011-01-01 03:00:00', '2011-01-01 03:20:00', 4, 3, 'mat5', 25,
'00:00:05', 25, 0.75),
('2011-01-01 04:00:00', '2011-01-01 04:40:00', 5, 2, 'mat3', 30,
'00:00:00', 0, 0.60),
('2011-01-01 05:00:00', '2011-01-01 05:30:00', 6, 4, 'mat5', 28,
'00:00:06', 30, 0.35),
('2011-01-01 06:00:00', '2011-01-01 06:45:00', 7, 5, 'mat7', 40,
'00:00:02', 10, 0.75),
('2011-01-01 07:00:00', '2011-01-01 07:10:00', 8, 7, 'mat8', 20,
'00:00:00', 0, 0.75),
('2011-01-01 08:00:00', '2011-01-01 08:30:00', 9, 8, 'mat9', 24,
'00:00:03', 15, 0.60),
('2011-05-01 00:00:00', '2011-05-01 02:00:00', 10, 4, 'mat10',
100, '00:00:25', 125, 0.75);




---- 1. Indique cuales son los autos con mayor cantidad de kilómetros realizados en el último mes.
create view EJ7.autos_mayor_km as
	select v.auto, sum(v.kmTotales) as km from EJ7.viaje v
	where v.fechaHoraInicio between dateAdd(month,-1,getDate()) and getDate();
select km.auto from autos_mayor_km kma where kma.km =(
	select max(kma.km) from autos_mayor_km kma
)

---- 2. Indique los clientes que más viajes hayan realizado con el mismo chofer.
create view EJ7.viajes_con_chofer
as
	select v.cliente, v.chofer, count(*) as cantidad
	from viaje v
	group by v.cliente, v.chofer

select vcc.cliente from EJ7.viajes_con_chofer vcc where	vcc.cantidad = (
	select max(v.cantidad) from EJ7.viajes_con_chofer v
)

---- 3. Indique el o los clientes con mayor cantidad de viajes en este año.
create view cliente_cant_viajes_anio 
as
	select v.cliente, count(*) as cantidad from EJ7.viaje v where year(date(v.fechaHoraInicio)) = year(getDate()) 
	group by v.cliente

select ccva.cliente from EJ7.cliente_cant_viajes_anio ccva where ccva.cantidad = (
	select max (ccva2.cantidad) from EJ7.cliente_cant_viajes_anio 
) 

---- 4.  Obtenga nombre y apellido de los choferes que no manejaron todos los vehículos que disponemos.
create view choferTodos
as
	select c.nroLicencia from EJ7.chofer c
	where not exists(
		select c.nroLicencia, a.matricula
		from EJ7.chofer c, EJ7.auto a
		where not exists(
			select v.chofer, v.auto from EJ7.viaje v
		)
	)
select c.nombre, c.apellido from EJ7.chofer c inner join EJ7.choferTodos ct on c.nroLicencia <> ct.nroLicencia

---- 5. Obtenga el nombre y apellido de los clientes que hayan viajado en todos nuestros autos.
---- reutilizo la view del 4)

select c.nombre, c.apellido from EJ7.chofer c inner join EJ7.choferTodos ct on c.nroLicencia = ct.nroLicencia

---- 6.  Queremos conocer el tiempo de espera promedio de los viajes de los últimos 2 meses
select avg(v.esperaTotal) as promedio from EJ7.viaje v where v.fechaHoraInicio between dateAdd(month,-2,getDate()) and getDate();

---- 7. Indique los kilómetros realizados en viajes por cada auto.
select sum(v.kmTotales) from EJ7.viaje v
group by v.auto

---- 8.Indique el costo promedio de los viajes realizados por cada auto.
select v.auto, avg(v.costoEspera+v.costoKms) from EJ7.viaje v
group by v.auto

---- 9. Indique el costo total de los viajes realizados por cada chofer en el último mes.
select v.chofer, sum(v.costoEspera+v.costoKms) from EJ7.viaje v
where v.fechaHoraInicio between dateAdd(month,-1,getDate()) and getDate()
group by v.chofer

---- 10. Indique la fecha inicial, el chofer y el cliente que hayan realizado el viaje más largo de este año.
select date(v.fechaHoraInicio), v.chofer, v.cliente from EJ7.viaje v
where (v.fechaHoraFin - v.fechaHoraInicio) = (
	select max(v.fechaHoraFin - v.fechaHoraInicio) from EJ7.viaje v
	where year(v.fechaHoraInicio) = year(getDate())
) 
