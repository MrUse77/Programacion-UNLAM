use Ejercicios;

create schema EJ8;

create table EJ8.persona
(
	persona VARCHAR(30),
	sexo VARCHAR(20),
	PRIMARY KEY(persona)
);

create table EJ8.bar
(
	bar VARCHAR(30),
	ciudad VARCHAR(30),
	PRIMARY KEY (bar)
);

create table EJ8.cerveza
(
	cerveza VARCHAR(30),
	tipo VARCHAR(15),
	PRIMARY KEY (cerveza)
);

create table EJ8.frecuenta
(
	persona VARCHAR(30),
	bar VARCHAR(30),
	PRIMARY KEY (persona, bar),
	FOREIGN KEY (persona) REFERENCES EJ8.persona(persona),
	FOREIGN KEY (bar) REFERENCES EJ8.bar(bar)
);

create table EJ8.sirve
(
	bar VARCHAR(30),
	cerveza VARCHAR(30),
	PRIMARY KEY (bar,cerveza),
	FOREIGN KEY(bar) REFERENCES EJ8.bar(bar),
	FOREIGN KEY(cerveza) REFERENCES EJ8.cerveza(cerveza)
);

create table EJ8.gusta
(
	persona VARCHAR(30),
	cerveza VARCHAR(30),
	PRIMARY KEY (persona,cerveza),
	FOREIGN KEY(persona) REFERENCES EJ8.persona(persona),
	FOREIGN KEY(cerveza) REFERENCES EJ8.cerveza(cerveza)
);

INSERT INTO EJ8.persona (persona,sexo) VALUES
('Persona1','Masculino'), 
('Persona2','Masculino'),
('Persona3','Femenino'),
('Persona4','Masculino'), 
('Persona5','Masculino'),
('Persona6','Masculino'),
('Persona7','Femenino'), 
('Persona8','Femenino'),
('Persona9','Masculino'),
('Persona10','Femenino'); 

INSERT INTO EJ8.bar (bar,ciudad) VALUES
('Bar1','Palermo'), ('Bar2','Palermo'), ('Bar3','Palermo'),
('Bar4','Belgrano'),('Bar5','Belgrano'), ('Bar6','Caballito'),
('Bar7','Caballito'), ('Bar8','Flores'), ('Bar9','San Telmo'),
('Bar10','Recoleta');

INSERT INTO EJ8.cerveza (cerveza,tipo) VALUES
('Cerveza1','rubia'), ('Cerveza2','rubia'),
('Cerveza3','rubia'),
('Cerveza4','negra'), ('Cerveza5','negra'), ('Cerveza6','roja'),
('Cerveza7','roja'), ('Cerveza8','rubia'), ('Cerveza9','negra'),
('Cerveza10','roja');

INSERT INTO EJ8.frecuenta (persona, bar) VALUES
('Persona1','Bar1'), ('Persona1','Bar2'), ('Persona2','Bar5'),
('Persona3','Bar3'), ('Persona3','Bar1'), ('Persona4','Bar6'),
('Persona4','Bar2'), ('Persona4','Bar3'), ('Persona4','Bar4'),
('Persona5','Bar7');

INSERT INTO EJ8.sirve (bar, cerveza) VALUES
('Bar1','Cerveza1'), ('Bar1','Cerveza2'), ('Bar1','Cerveza3'),
('Bar2','Cerveza4'), ('Bar2','Cerveza3'), ('Bar3','Cerveza1'),
('Bar7','Cerveza2'), ('Bar4','Cerveza1'), ('Bar5','Cerveza1'),
('Bar6','Cerveza3');

INSERT INTO EJ8.gusta (persona, cerveza) VALUES
('Persona1','Cerveza1'), ('Persona1','Cerveza3'),
('Persona2','Cerveza4'),
('Persona3','Cerveza2'), ('Persona4','Cerveza1'),
('Persona4','Cerveza2'),
('Persona4','Cerveza3'), ('Persona4','Cerveza4'),
('Persona5','Cerveza2'),
('Persona5','Cerveza4');

---- 1. Frecuentan solamente bares que sirven alguna cerveza que les guste
select distinct f.persona from EJ8.frecuenta f
where not exists(
	select f.persona, f.bar from EJ8.frecuenta f
	where not exists(
		select g.persona,s.bar from EJ8.gusta g inner join EJ8.sirve s on g.cerveza = s.cerveza
	)
);

---- 2. No frecuentan ningún bar que sirva alguna cerveza que les guste.
select  f.persona from EJ8.frecuenta f
where not exists(
	select f.persona from EJ8.frecuenta f
	inner join EJ8.gusta g on f.persona = g.persona
	inner join EJ8.sirve s on f.bar = s.bar
	and s.cerveza = g.cerveza
);

---- 3.  Frecuentan solamente los bares que sirven todas las cervezas que les gustan.
select distinct f.persona from EJ8.frecuenta f
where not exists(
	select p.persona  from (
		select f.persona, f.bar, g.cerveza
		from EJ8.frecuenta f
		inner join EJ8.gusta g on f.persona = g.persona 
		where not exists(
			select f.persona, f.bar, s.cerveza
			from EJ8.frecuenta f
			inner join EJ8.sirve s on f.bar=s.bar
		)
	)p
)

---- 4. Frecuentan solamente los bares que no sirven ninguna de las cervezas que no les gusta
create view EJ8.noGusta 
as 
	select f.persona, s.cerveza from EJ8.frecuenta, sirve s
	where not exits(
		select g.persona,g.cerveza from gusta g
	)
select view personaNo
as
	select f.persona from EJ8.frecuenta f
	inner join EJ8.sirve s on f.bar = s.bar
	inner join EJ8.noGusta ng f.persona = ng.persona 
	and s.cerveza = ng.cerveza
