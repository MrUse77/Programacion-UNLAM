-- crear la base de datos
create database ej13_Metricas;

-- usar la base de datos
use ej13_Metricas;

-- 1
drop procedure p_CrearEntidades;

create procedure p_CrearEntidades
as
	begin try
		begin transaction
			create table nivel
			(
				codigo int not null,
				descripcion varchar(30) not null,
				primary key(codigo)
			);

			create table medicion
			(
				fecha date not null,
				hora time not null,
				metrica char(5) not null,
				temperatura float not null,
				presion float not null,
				humedad float not null,
				nivel int not null,
				primary key(fecha, hora, metrica),
				foreign key(nivel) references nivel(codigo)
			);
		commit
	end try
	begin catch
		rollback
	end catch;

execute p_CrearEntidades
