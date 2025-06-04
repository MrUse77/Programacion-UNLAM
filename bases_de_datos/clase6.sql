CREATE TABLE Alumno (
    id int identity(1,1) PRIMARY KEY,
    nombre VARCHAR(50),
    apellido VARCHAR(50),
    fecha_nacimiento DATE,
    dni VARCHAR(15) UNIQUE,
    email VARCHAR(100)
);
 
CREATE TABLE Materia (
    id int identity(1,1) PRIMARY KEY,
    nombre VARCHAR(100)
);
 
CREATE TABLE Cursada (
    id int identity(1,1) PRIMARY KEY,
    id_alumno INT,
    id_materia INT,
    anio INT,
    FOREIGN KEY (id_alumno) REFERENCES Alumno(id) ON DELETE CASCADE,
    FOREIGN KEY (id_materia) REFERENCES Materia(id) ON DELETE CASCADE
);
 
CREATE TABLE TipoEvaluacion (
    id int identity(1,1) PRIMARY KEY,
    descripcion VARCHAR(50)
);
CREATE TABLE Evaluacion (
    id int identity(1,1) PRIMARY KEY,
    id_cursada INT,
    id_tipo_evaluacion INT,
    fecha DATE,
    nota DECIMAL(4, 2),
    FOREIGN KEY (id_cursada) REFERENCES Cursada(id) ON DELETE CASCADE,
    FOREIGN KEY (id_tipo_evaluacion) REFERENCES TipoEvaluacion(id)
);

-- Crear un SP para listar alumnos que nunca desaprobaron (usar procedure)

create or alter procedure ListarAlumnosSinDesaprobados as
  begin
    select distinct a.id, a.nombre, a.apellido, a.fecha_nacimiento, a.dni, a.email
    from Alumno a
    where not exists (
      select 1 from Cursada c
      inner join Evaluacion e on c.id = e.id_cursada
      where e.nota < 4 and c.id_alumno = a.id
    )
  end;


execute ListarAlumnosSinDesaprobados;

-- Listar alumnos que aprobaron toda materia que existe
create or alter procedure ListarAlumnosAprobados as
  begin
    select distinct a.id, a.nombre, a.apellido, a.fecha_nacimiento, a.dni, a.email
    from Alumno a
    where not exists (
      select 1 from Materia m
      where not exists (
      select 1 from Cursada c
      inner join Evaluacion e on c.id = e.id_cursada
      where c.id_alumno = a.id and c.id_materia = m.id and e.nota >= 4
      )
    ) 
  end;

execute ListarAlumnosAprobados;
