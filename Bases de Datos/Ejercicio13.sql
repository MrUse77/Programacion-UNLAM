/*
Medición(fecha,hora,métrica,temperatura,presión,humedad,nivel)
Nivel (código, descripción)
Realizar las siguientes consultas utilizando lenguaje SQL:*/

CREATE DATABASE Ejercicio13;
use Ejercicio13;


/*1. p_CrearEntidades(): Realizar un procedimiento que permita crear las tablas de nuestro modelo relaciona*/
CREATE PROCEDURE p_CrearEntidades AS
BEGIN
CREATE TABLE Nivel (
    código INT PRIMARY KEY,
    descripción VARCHAR(255)
);
CREATE TABLE Medición (
    fecha DATE,
    hora TIME,
    métrica VARCHAR(255),
    temperatura FLOAT,
    presión FLOAT,
    humedad FLOAT,
    nivel INT,
    FOREIGN KEY (nivel) REFERENCES Nivel (código)
);
END

/*2. f_ultimamedicion(Métrica): Realizar una función que devuelva la fecha y hora de la última 
medición realizada en una métrica específica, la cual será enviada
por parámetro. La sintaxis de la función deberá respetar lo siguiente: Fecha/hora = f_ultimamedicion(vMetrica char(5)) 
Ejemplificar el uso de la función.*/

CREATR FUNCTION f_ultimaedicion (vMetrica VARCHAR(5)) RETURN DATETIME AS
BEGIN 
    DECLARE vFechaHora DATETIME;
    SELECT MAX(CONCAT(fecha, ' ', hora)) INTO vFechaHora
    FROM Medición
    WHERE métrica = vMetrica;
    RETURN vFechaHora;
END;

SELECT f_ultimaedicion('M1') AS UltimaMedicion;

/*3. v_Listado: Realizar una vista que permita listar las métricas en las cuales se
hayan realizado, en la última semana, mediciones para todos los niveles
existentes. El resultado del listado deberá mostrar, el nombre de la métrica que
respete la condición enunciada, el máximo nivel de temperatura de la última
semana y la cantidad de mediciones realizadas también en la última semana.*/

CREATE VIEW v_Listado AS
BEGIN
    SELECT m.métrica, MAX(m.temperatura) AS MaxTemperatura, COUNT(*) AS CantidadMediciones
    FROM Medición m
    WHERE m.fecha >= DATE_SUB(CURDATE(), INTERVAL 7 DAY)
    GROUP BY m.métrica
    HAVING COUNT(DISTINCT m.nivel) = (SELECT COUNT(*) FROM Nivel);
END;

/*p_ListaAcumulados(finicio,ffin): Realizar un procedimiento que permita
generar una tabla de acumulados diarios de temperatura por cada métrica y por
cada día. El procedimiento deberá admitir como parámetro el rango de fechas
que mostrará el reporte. Además, estas fechas deben ser validadas.*/

CREATE PROCEDURE p_ListaAcumulados (finicio DATE, ffin DATE) AS
BEGIN
    SELECT m.fecha, m.métrica, 
	   SUM(m.temperatura) AS AcumuladoDiarioTemp,
	   (SELECT SUM(m2.temperatura) 
	    FROM Medición m2 
	    WHERE m2.métrica = m.métrica AND m2.fecha BETWEEN finicio AND ffin) AS AcumuladoTemp
    FROM Medición m 
    WHERE m.fecha BETWEEN finicio AND ffin
    GROUP BY m.fecha, m.métrica;
END;

/*CREATE PROCEDURE p_ListaAcumulados (finicio DATE, ffin DATE) AS
BEGIN
	SELECT m.fecha,m.metrica, SUM(m.temperatura) as AcDiarioTemp, SUM(SELECT SUM(m2.temperatura) FROM Medicion m2 WHERE m2.metrica = m.metrica AND m2.fecha BETWEEN finicio AND ffin) as Ac.Temp
	FROM Medicion m WHERE m.fecha BETWEEN finicio AND ffin
END*/



