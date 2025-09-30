use Ejercicios;
create schema EJ11;
---- 1 Realice las sentencias DDL necesarias para crear en SQL una base de datos correspondiente al modelo relacional del enunciado.
create table EJ11.pelicula(
	codPel int not null primary key,
	titulo varchar(50) not null,
	duracion smalldatetime not null,
	codGenero int not null,
	idDirector int not null,
	foreign key(codGenero) references EJ11.genero(id),
	foreign key(idDirector) references EJ11.director(id),
);

create table EJ11.genero(
	id int not null primary key,
	nomGenero varchar(50) not null,
);

create table EJ11.director(
	id int not null primary key,
	NyA varchar(50) not null,
);

create table EJ11.ejemplar(
	nroEj int not null,
	codPel int not null,
	estado bit not null,
	primary key(nroEj,codPel),
	foreign key(codPel) references EJ11.pelicula(codPel)
);

create table EJ11.cliente(
	codCli int not null,
	NyA varchar(50) not null,
	direccion varchar(50) not null,
	tel int not null,
	email varchar(50) not null,
	borrado int DEFAULT 2,
	primary key(codCli)
);

create table EJ11.alquiler(
	id int not null,
	nroEj int not null,
	codPel int not null,
	codCli int not null,
	fechaAlq date not null,
	fechaDev date not null,
	primary key(id),
	foreign key(nroEj,codPel) references EJ11.ejemplar(nroEj,codPel),
	foreign key(codCli) references EJ11.cliente(codCli),
);
---- 2. Realice los INSERTs necesarios para cargar en las tablas creadas en el punto anterior los datos de 5 clientes, 10 peliculas (y tablas relacionadas a estas) y al menos 15 alquileres.

INSERT INTO EJ11.genero (id, nomGenero) VALUES (1,'Terror');
INSERT INTO EJ11.genero (id, nomGenero) VALUES (2,'Comedia');
INSERT INTO EJ11.genero (id, nomGenero) VALUES (3,'Drama');
INSERT INTO EJ11.genero (id, nomGenero) VALUES (4,'Accion');
INSERT INTO EJ11.genero (id, nomGenero) VALUES (5,'Infantil');
INSERT INTO EJ11.genero (id, nomGenero) VALUES (6,'Ciencia
Ficción');
INSERT INTO EJ11.director (id, NyA) VALUES (1,'David Lynch');
INSERT INTO EJ11.director (id, NyA) VALUES (2,'Martin Scorsese');
INSERT INTO EJ11.director (id, NyA) VALUES (3,'Pedro Almodovar');
INSERT INTO EJ11.director (id, NyA) VALUES (4,'Quentin Tarantino');
INSERT INTO EJ11.director (id, NyA) VALUES (5,'Larry and Andy
Wachowski');
INSERT INTO EJ11.director (id, NyA) VALUES (6,'Clint Eastwood');
INSERT INTO EJ11.director (id, NyA) VALUES (7,'James Cameron');
INSERT INTO EJ11.director (id, NyA) VALUES (8,'Steven Spielberg');
INSERT INTO EJ11.cliente (codCli, NyA, direccion, tel, email,
borrado)
VALUES (1,'Cosme, Fulanito', 'CalleFalsa 123', '3344-5325',
'cosme@email.com', 2);
INSERT INTO EJ11.cliente(codCli, NyA, direccion, tel, email,
borrado)
VALUES (2,'Perez, Jorge', 'Cerrito 223', '9834-3385',
'perez@email.com', 2);
INSERT INTO EJ11.cliente (codCli, NyA, direccion, tel, email,
borrado)
VALUES (3,'Suarez, Pepe', 'Uruguay 2322', '4594-9482',
'suarez@email.com', 2);
INSERT INTO EJ11.cliente (codCli, NyA, direccion, tel, email,
borrado)
VALUES (4,'Fernandez, Juancito', 'Pueyrredon 2343', '7833-5893',
'fernandez@email.com', 2);
INSERT INTO EJ11.cliente (codCli, NyA, direccion, tel, email,
borrado)
VALUES (5,'Torres, Pepe', 'Rivadavia 7897', '7484-3298',
'torres@email.com', 2);
INSERT INTO EJ11.pelicula(codPel, titulo, duracion, codGenero,
idDirector)
VALUES (1,'Terminator', '1:30:00', 4, 1);
INSERT INTO EJ11.pelicula(codPel, titulo, duracion, codGenero,
idDirector)
VALUES (2,'Avatar', '2:30:00', 4, 7);
INSERT INTO EJ11.pelicula(codPel, titulo, duracion, codGenero,
idDirector)
VALUES (3,'Kill Bill', '1:45:00', 4, 4);
INSERT INTO EJ11.pelicula(codPel, titulo, duracion, codGenero,
idDirector)
VALUES (4,'Matrix', '1:30:00', 4, 5);
INSERT INTO EJ11.pelicula(codPel, titulo, duracion, codGenero,
idDirector)
VALUES (5,'Volver al Futuro', '1:20:00', 5, 8);
INSERT INTO EJ11.pelicula(codPel, titulo, duracion, codGenero,
idDirector)
VALUES (6,'300', '1:40:00', 4, 3);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (1,1,1);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (2,1,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (1,2,1);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (2,2,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (1,3,1);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (2,3,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (1,4,1);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (2,4,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (1,5,1);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (2,5,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (1,6,1);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (2,6,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (3,2,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (4,2,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (5,2,0);
INSERT INTO EJ11.ejemplar (nroEj,codPel,estado) VALUES (6,2,0);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (1,1,1,4,'2011-06-11', Null);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (2,1,2,2,'2011-06-15', Null);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (3,1,3,3,'2011-06-22',Null);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (4,1,4,5,'2011-06-30',Null);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (5,1,5,4,'2011-07-01',Null);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (6,1,6,1,'2011-07-06',Null);
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (7,1,2,4,'2011-05-11','2011-05-13');
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (8,2,2,2,'2011-05-15','2011-05-17');
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (9,3,2,3,'2011-06-12','2011-06-14');
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (10,4,2,5,'2011-06-23','2011-06-25');
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (11,5,2,4,'2011-06-06','2011-06-08');
INSERT INTO EJ11.alquiler (id, nroEj, codPel, codCli, fechaAlq,
fechaDev) VALUES (12,6,2,1,'2011-06-22','2011-06-24');

---- 3. Agregue el atributo “año” en la tabla Película
alter table EJ11.pelicula add column año int null;

---- 4.Actualice la tabla película para que incluya el año de lanzamiento de las películas en stock.
create view EJ11.añoLanz
as
	select e.codPel from EJ11.ejemplar e
	where e.estado = 0
	group by e.codPel
	having count(*) > 0;

---- 5. Queremos que al momento de eliminar una película se eliminen todos los ejemplares de la misma. Realice una CONSTRAINT para esta tarea
create trigger EJ11.eliminarPelicula
on EJ11.pelicula
	before delete
	as
	begin
		delete from EJ11.ejemplar
		where codPel = deleted.codPel
	end;

---- 6. Queremos que exista un borrado de lógico y no físico de clientes, realice un TRIGGER que usando el atributo “Borrado” haga esta tarea.
create trigger EJ11.borradoLogico
on EJ11.cliente instead of delete
	as 
	begin 
		update cliente 
		set borrado = 1 where codCli = deleted.codCli
	end;

---- 7. Elimine las películas de las que no se hayan alquilado ninguna copia.
create procedure peliculasNoAlquiladas
as
begin try
	begin transaction
		delete  from EJ11.pelicula p where not exists(
			select a.codPel from EJ11.alquiler a 
		)
	commit 
end try
begin catch
	rollback
end catch

---- 8. Elimine los clientes sin alquileres.

create procedure clientesSinAlquiler
as
begin try
	begin transaction
		delete  from EJ11.cliente where not exists(
			select a.codCli from EJ11.alquiler a 
		)
	commit 
end try
begin catch
	rollback
end catch

 
