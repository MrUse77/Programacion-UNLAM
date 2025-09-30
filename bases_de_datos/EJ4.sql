use Ejercicios;

create schema EJ4;

create table EJ4.persona(
	dni int not null,
	nomPersona varchar(50) not null,
	telefono varchar(15) not null,
	primary key(dni)
);

create table EJ4.empresa(
	nomEmpresa varchar(50) not null,
	telefono varchar not null,
	primary key(nomEmpresa)
);

create table EJ4.vive(
	dni int not null,
	calle varchar(50) not null,
	ciudad varchar(50) not null,
	constraint FKdni foreign key(dni) references EJ4.persona(dni)
);

create table EJ4.trabaja(
	dni int not null,
	nomEmpresa varchar(50) not null,
	salario int not null,
	feIngreso date not null,
	feEgreso date not null,
	constraint FKdni foreign key(dni) references EJ4.persona(dni)
	constraint FKEmpresa foreign key(nomEmpresa) references EJ4.empresa(nomEmpresa)
);

create table EJ4.situada_en(
	nomEmpresa varchar(50) not null,
	ciudad varchar(50) not null,
	constraint FKEmpresa foreign key(nomEmpresa) references EJ4.empresa(nomEmpresa)
);

create table EJ4.supervisa(
	dniPer int not null,
	dniSup int not null,
	constraint PKSupervisa primary key(dniPer,dniSup)
	constraint FKPer foreign key(dniPer) references EJ4.persona(dni),
	constraint FKSup foreign key(dniSup) references EJ4.persona(dni)
);

---- 1.Encontrar el nombre de todas las personas que trabajan en la empresa “Banelco”.
select p.nomPersona from EJ4.persona p where p.dni in(
	select t.dni from EJ4.trabaja t where t.nomEmpresa = "Banelco" 
)

---- 2.Localizar el nombre y la ciudad de todas las personas que trabajan para la empresa “Telecom”.
select distinct v.ciudad as Ciudad, p.nomPersona as Persona from EJ4.Trabaja t
	inner join EJ4.vive v on t.dni = v.dni
	inner join EJ4.persona p on t.dni = p.dni
	where t.nomEmpresa = "Telecom"
	order by v.ciudad, p.nomPersona

---- 3. uscar el nombre, calle y ciudad de todas las personas que trabajan para la empresa “Paulinas” y ganan más de $1500.
select p.nomPersona, v.calle, v.ciudad from EJ4.trabaja t
	inner join EJ4.persona p on t.dni = p.dni
	inner join EJ4.vive v on t.dni = v.dni
	where t.nomEmpresa = "Paulinas" and t.salario > 1500

---- 4.Encontrar las personas que viven en la misma ciudad en la que se halla la empresa en donde trabajan
select * from EJ4.persona p where p.dni in(
	select t.dni from EJ4.trabaja t
	inner join EJ4.situada_en s on t.nomEmpresa = s.nomEmpresa
	inner join EJ4.vive v on t.dni = v.dni
	where s.ciudad = v.ciudad
)

---- 5. Hallar todas las personas que viven en la misma ciudad y en la misma calle que su supervisor
select * from EJ4.persona p 
	inner join EJ4.vive v on v.dni = p.dni
	where exists(
		select 1 from EJ4.supervisa s
		inner join EJ4.vive v2 on s.dniSup = v2.dni 
		where v2.calle = v.calle and v2.ciudad = v.ciudad and s.dniPer = p.dni
	)

---- 6. Encontrar todas las personas que ganan más que cualquier empleado de la empresa “Clarín”
select p.* from EJ4.persona p
	inner join EJ4.trabaja t on p.dni = t.dni
	where not exists(
		select 1 from EJ4.trabaja t2 where 
		t2.nomEmpresa = 'Clarin' and t.salario <= t2.salario
	)

---- 7. Localizar las ciudades en las que todos los trabajadores que vienen en ellas ganan más de $1000.
select v.ciudad from EJ4.vive v where not exists(
	select 1 from EJ4.trabaja t where t.dni = v.dni and t.salario <= 1000
)
group by v.ciudad

---- 8. Listar los primeros empleados que la compañía “Sony” contrató.
select * from EJ4.persona p
	inner join EJ4.trabaja t on p.dni = t.dni
	where t.nomEmpresa = 'Sony' and t.feIngreso = (
		select min(t2.feIngreso) from EJ4.trabaja t2 where t2.nomEmpresa = 'Sony' 
	)

---- 9.Listar los empleados que hayan ingresado en mas de 4 Empresas en el periodo 01-01-2000 y 31-03-2004 y que no hayan tenido menos de 5 supervisores
select * from EJ4.persona p where p.dni in (
	select t.dni from EJ4.trabaja t 
	where t.feIngreso between '2000-01-01' and '2004-03-31'
	group by t.nomEmpresa
	having count(distinct t.nomEmpresa)>4
) and p.dni in(
	select s.dniPer from EJ4.supervisa
	group by s.dniPer
	having count(distinct s.dniSuo)>=5
)
