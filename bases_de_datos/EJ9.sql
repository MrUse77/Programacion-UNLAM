use Ejercicios;


create schema EJ9;

create table EJ9.persona(
	tipo_doc VARCHAR(3),
	nro_doc BIGINT,
	nombre VARCHAR(50),
	direccion VARCHAR(50),
	fecha_nac DATETIME,
	sexo VARCHAR(9),
	PRIMARY KEY (tipo_doc,nro_doc)
);

create table EJ9.progenitor(
	tipo_doc VARCHAR(3),
	nro_doc BIGINT,
	t_doc_hijo VARCHAR(3),
	n_doc_hijo BIGINT,
	PRIMARY KEY (tipo_doc,nro_doc,t_doc_hijo,n_doc_hijo),
	FOREIGN KEY (tipo_doc,nro_doc) REFERENCES EJ9.persona(tipo_doc,nro_doc),
	FOREIGN KEY (t_doc_hijo,n_doc_hijo) REFERENCES EJ9.persona(tipo_doc,nro_doc)
);

insert into EJ9.persona(tipo_doc,nro_doc,nombre,direccion,fecha_nac,sexo)
VALUES
('LC',12345670,'Persona1','Direccion1','19700302','Masculino'),
('DNI',12345671,'Persona2','Direccion2','19800302','Femenino'),
('LC',12345672,'Persona3','Direccion3','19790602','Masculino'),
('DNI',12345673,'Persona4','Direccion4','19890310','Masculino'),
('LC',12345674,'Persona5','Direccion5','19700302','Femenino'),
('DNI',12345675,'Persona6','Direccion6','19881013','Masculino'),
('LC',12345676,'Persona7','Direccion7','19751223','Femenino'),
('DNI',12345677,'Persona8','Direccion8','19790704','Femenino'),
('DNI',12345678,'Persona9','Direccion9','19831005','Masculino'),
('DNI',12345679,'Persona10','Direccion10','19800312','Masculino'
);


insert into EJ9.progenitor(tipo_doc,nro_doc,t_doc_hijo,n_doc_hijo)
VALUES
('LC',12345670,'DNI',12345671),('LC',12345672,'DNI',12345679),
('LC',12345670,'DNI',12345679),('LC',12345676,'DNI',12345678),
('LC',12345676,'DNI',12345677),('LC',12345674,'DNI',12345675),
('LC',12345674,'DNI',12345677),('LC',12345674,'DNI',12345678),
('LC',12345670,'DNI',12345675),('LC',12345672,'DNI',12345678);

---- 1. Hallar para una persona dada, por ejemplo José Pérez, los tipos y números de documentos, nombres, dirección y fecha de nacimiento de todos sus hijos
select p.tipo_doc, p.nro_doc, p.nombre,p.direccion,p.fecha_nac from EJ9.progenitor prog
right join EJ9.persona p on prog.t_doc_hijo = p.tipo_doc and prog.n_doc_hijo = p.nro_doc
where prog.tipo_doc = 'LC' and prog.nro_doc='12345670'

---- 2.Hallar para cada persona los tipos y números de documento, nombre, domicilio y fecha de nacimiento de:
---- a. Todos sus hermanos, incluyendo medios hermanos.
create view EJ9.hermanos
as 
	select p1.tipo_doc as tipoDocP,p1.nro_doc as nroDocP,p2.tipo_doc as tipoDoc,p2.nro_doc as nroDoc from EJ9.progenitor p1, EJ9.progenitor p2
	where p1.tipo_doc=p2.tipo_doc and p1.nro_doc = p2.nro_doc and p1.t_doc_hijo <> p2.t_doc_hijo and p1.n_doc_hijo <> p2.n_doc_hijo;

select h.tipoDocP, h.nroDocP, h.tipoDoc, h.nroDoc, p.nombre, p.direccion, p.fecha_nac from EJ9.hermanos h
inner join EJ9.persona p on h.tipoDoc = p.tipo_doc and h.nroDoc = p.nro_doc; 

---- b. Su madre
create view EJ9.hijo
as
	select p.tipo_doc as tipoDoc, p.nro_doc as nroDoc, p.t_doc_hijo as tipoDocH, p.n_doc_hijo as nroDocH from EJ9.progenitor p;

select m.tipoDoc, m.nroDoc, p.nombre, p.direccion, p.fecha_nac from EJ9.hijoP m 
inner join EJ9.persona p on m.tipoDoc = p.tipo_doc and m.nroDoc = p.nro_doc
where p.sexo = 'Femenino';

---- c. Su abuelo materno
create view EJ9.hijoM
as 
	select h.tipoDoc, h.nroDoc,h.tipoDocH,h.nroDocH, p.nombre, p.direccion, p.fecha_nac from EJ9.hijo h 
	inner join EJ9.persona p on h.tipoDoc = p.tipo_doc and h.nroDoc = p.nro_doc
	where p.sexo = 'Femenino';

create view EJ9.hijoP
as
	select h.tipoDoc, h.nroDoc,h.tipoDocH,h.nroDocH, p.nombre, p.direccion, p.fecha_nac from EJ9.hijo h 
	inner join EJ9.persona p on h.tipoDoc = p.tipo_doc and h.nroDoc = p.nro_doc
	where p.sexo = 'Masculino';


select a.tipoDoc, a.nroDoc, a.nombre, a.direccion, a.fecha_nac from EJ9.hijoM m 
inner join EJ9.hijoP a on m.tipoDoc = a.tipoDocH and m.nroDoc = a.nroDocH

---- d.Todos sus nietos
create view EJ9.nieto
as
	select n.tipoDocH as tipoDocN, n.nroDocH as nroDocN from EJ9.hijo a
	inner join EJ9.hijo prog on a.tipoDocH = prog.tipoDoc and a.nroDocH = prog.nroDoc
	inner join EJ9.hijo n on prog.tipoDocH = n.tipoDoc and prog.nroDocH = n.nroDoc

select n.tipoDocN, n.nroDocN,p.nombre, p.direccion, p.fecha_nac from EJ9.nieto n 
inner join EJ9.persona p on n.tipoDocN = p.tipo_doc and n.nroDocN = p.nro_doc



