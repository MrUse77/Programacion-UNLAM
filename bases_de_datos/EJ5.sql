use Ejercicios;

create schema EJ5;

create table EJ5.pelicula(
	codPel int not null,
	titulo varchar(50) not null,
	duracion int not null,
	año date not null,
	codRubro int not null,
	primary key(codPel),
	constraint FKRubro foreign key(codRubro) references EJ5.rubro(codRubro)
);

create table EJ5.rubro(
	codRubro int not null primary key,
	nomRubro varchar(50) not null,
);

create table EJ5.ejemplar(
	codEj int not null,
	codPel int not null,
	estado varchar(50) not null,
	ubicacion varchar(50) not null,
	constraint PKEjemplar primary key(codEj,codPel),
	constraint FKPel foreign key(codPel) references EJ5.pellicula(codPel)
);

create table EJ5.cliente(
	codCli int not null primary key,
	nombre varchar(50) not null,
	apellido varchar(50)not null,
	direccion varchar(50) not null,
	tel varchar(50) not null,
	email varchar(50) not null
);
create table EJ5.prestamo(
	codPrest int not null primary key,
	codEj int not null,
	codPel int not null,
	codCli int not null,
	fechaPrest date not null,
	fechaDev date,
	constraint FKEjemplar foreign key(codEj, codPel) references EJ5.ejemplar(codEj, codPel),

	constraint FKPelicula foreign key(codPel) references EJ5.pelicula(codPel),
	constraint FKCliente foreign key(codCli) references EJ5.cliente(codCli),
);

---- 1. Listar los clientes que no hayan reportado préstamos del rubro “Policial”.
select * from EJ5.cliente c where not exists(
	select 1 from EJ5.prestamos p
	inner join EJ5.pelicula pe on p.codPel = pe.codPel
	inner join EJ5.rubro r on pe.codRubro = r.codRubro
	where r.nomRubro = 'Policial' and c.codCli = p.codCli
)

---- 2. Listar las películas de mayor duración que alguna vez fueron prestadas.
select * from EJ5.pelicula p 
	where p.duracion = (select max(p2.duracion) from EJ5.pelicula p inner join EJ5.prestamo pe on p2.codPel = pe.codPel)
	and exists(
		select 1 from EJ5.prestamo pe2 
			where p.codPel = pe2.codPel
	)

---- 3. Listar los clientes que tienen más de un préstamo sobre la misma película (listar Cliente, Película y cantidad de préstamos).
select c.nombre as cliente, p.titulo as pelicula, count(*) from EJ5.cliente c
	inner join EJ5.prestamo pe on c.codCli = pe.codCli
	inner join EJ5.pelicula p on pe.codPel = p.codPel
	group by c.nombre, p.titulo
	having count(*)>1

---- 4. Listar los clientes que han realizado préstamos del título “Rey León” y “Terminador 3” (Ambos).
select c.* from EJ5.clientes c
	where c.codCli in(
		select pe.codCli from EJ5.prestamo pe
		inner join EJ5.pelicula p on pe.codPel = p.codPel
		where p.titulo = 'Rey León'
	) and c.codCli in (
		select pe.codCli from EJ5.prestamo pe
		inner join EJ5.pelicula p on pe.codPel = p.codPel
		where p.titulo = 'Terminador 3'
	)

---- 5.Listar las películas más vistas en cada mes (Mes, Película, Cantidad de Alquileres).
select month(pe.fechaPrest) as mes, p.titulo as pelicula, count(*) as cantidad from EJ5.prestamo pe
	inner join EJ5.pelicula p on pe.codPel = p.codPel
	group by month(pe.fechaPrest), p.titulo
	having count(*) = (
		select max(cantidad) from (
			select count(*) as cantidad from EJ5.prestamo pe2
			inner join EJ5.pelicula p2 on pe2.codPel = p2.codPel
			where month(pe2.fechaPrest) = month(pe.fechaPrest)
			group by p2.titulo
		) as subquery
	)

---- 6. Listar los clientes que hayan alquilado todas las películas del video.
select c.* from EJ5.cliente c where not exists (
	select p.codPel from EJ5.pelicula p where not exists(
		select 1 from EJ5.prestamo pe where p.codPel = pe.codPel and c.codCli = pe.codCli
	)
)

---- 7.Listar las películas que no han registrado ningún préstamo a la fecha.
select * from EJ5.pelicula p where not exists(
	select 1 from EJ5.prestamo pe where p.codPel = pe.codPel
) 

