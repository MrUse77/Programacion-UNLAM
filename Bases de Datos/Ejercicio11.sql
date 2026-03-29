/*
 Película (CodPel, Titulo, Duracion, CodGenero, IdDirector)
Genero (Id, NombGenero)
Director (Id, NyA)
Ejemplar (nroEj, CodPel, Estado)
{Estado: 1 Disponible, 0 No disponible}
Cliente (CodCli, NyA, Direccion, Tel, Email, Borrado)
{Borrado: 1 Si, 2 No(Default) }
Alquiler (id, NroEj, CodPel, CodCli, FechaAlq, FechaDev)

CREATE DATABASE Ejercicio11;
*/
/*1. Realice las sentencias DDL necesarias para crear en SQL una base de datos correspondiente al modelo relacional del enunciado.*/
-- Default

use Ejercicio11;
CREATE TABLE Genero
(
    Id INT NOT NULL PRIMARY KEY,
    NombGenero VARCHAR(50) NOT NULL
);
CREATE TABLE Director
(
    Id INT NOT NULL PRIMARY KEY,
    NyA VARCHAR(100) NOT NULL
);
CREATE TABLE Cliente
(
    CodCli INT NOT NULL PRIMARY KEY,
    NyA VARCHAR(100) NOT NULL,
    Direccion VARCHAR(150) NOT NULL,
    Tel VARCHAR(20) NOT NULL,
    Email VARCHAR(100) NOT NULL,
    Borrado BIT NOT NULL,
    CONSTRAINT DF_Cliente_Borrado DEFAULT 0 FOR Borrado,
    CONSTRAINT CK_Cliente_Borrado CHECK (Borrado IN (0,1))
);
CREATE TABLE Pelicula
(
    CodPel INT NOT NULL PRIMARY KEY,
    Titulo VARCHAR(100) NOT NULL,
    Duracion INT NOT NULL,
    CodGenero INT NOT NULL,
    IdDirector INT NOT NULL,
    FOREIGN KEY (CodGenero) REFERENCES Genero(Id),
    FOREIGN KEY (IdDirector) REFERENCES Director(Id)
);
CREATE TABLE Ejemplar
(
    nroEj INT NOT NULL PRIMARY KEY,
    CodPel INT NOT NULL,
    Estado TINYINT NOT NULL CHECK (Estado IN (0, 1))
);
CREATE TABLE Alquiler
(
    id INT NOT NULL PRIMARY KEY,
    NroEj INT NOT NULL,
    CodPel INT NOT NULL,
    CodCli INT NOT NULL,
    FechaAlq DATE NOT NULL,
    FechaDev DATE,
    FOREIGN KEY (NroEj) REFERENCES Ejemplar(nroEj),
    FOREIGN KEY (CodPel) REFERENCES Pelicula(CodPel),
    FOREIGN KEY (CodCli) REFERENCES Cliente(CodCli)
);

/*2. Realice los INSERTs necesarios para cargar en las tablas creadas en el punto anterior los datos de 5 clientes, 10 peliculas (y tablas relacionadas a estas) y al menos 15 alquileres.*/
INSERT INTO Cliente(CodCli, NyA, Direccion, Tel, Email) VALUES
(1, 'Juan Perez', 'Calle Falsa 123', '123456789', 'a@mail.com'),(2, 'Maria Gomez', 'Avenida Siempre Viva 742', '987654321', 'b@mail'),
(3, 'Carlos Lopez', 'Boulevard Central 456', '456123789', 'c@mail'),(4, 'Ana Martinez', 'Calle Real 789', '321654987', 'd@mail'),
(5, 'Luis Rodriguez', 'Avenida Libertad 101', '654987321', 'e@mail');
SELECT * FROM Cliente;


/*3. Agregue el atributo “año” en la tabla Película.*/
ALTER TABLE Pelicula ADD Año SMALLINT NOT NULL;

/*4. Actualice la tabla película para que incluya el año de lanzamiento de las películas en stock.*/


/*5. Queremos que al momento de eliminar una película se eliminen todos los ejemplares de la misma. Realice una CONSTRAINT para esta tarea.*/
ALTER TABLE Ejemplar ADD CONSTRAINT fk_pelicula_ejemplar FOREIGN KEY (CodPel) REFERENCES Pelicula(CodPel) ON DELETE CASCADE;


/*6. Queremos que exista un borrado de lógico y no físico de clientes, realice un TRIGGER que usando el atributo “Borrado” haga esta tarea.*/
CREATE TRIGGER trg_borrado_logico_cliente ON Cliente INSTEAD OF DELETE AS
	BEGIN
	    UPDATE Cliente
	    SET Borrado = 1
	    WHERE CodCli IN (SELECT CodCli FROM DELETED);
	END;
/*Ejemplo de uso del trigger*/
DELETE FROM Cliente WHERE CodCli = 3;
SELECT * FROM Cliente;

/*7. Elimine las películas de las que no se hayan alquilado ninguna copia.*/
DEELETE FROM Pelicula WHERE CodPel NOT INT(SELECT DISTINCT CodPel FROM Alquiler);


/*8. Elimine los clientes sin alquileres.*/
DELETE FROM Cliente WHERE CodCli NOT IN (SELECT DISTINCT CodCli FROM Alquiler);


